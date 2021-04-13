import numpy as np
import matplotlib.pyplot as plt
import os
import math

# import data
# data = np.loadtxt(open("radar_fused.csv", "rb"), delimiter=";", skiprows=1, dtype=np.float64)

# resets the csv file including the first row
open('data/radar_fused.csv', 'w').close()

REMOVE_OLD_TRACKS_TIME_CONSTANT = 10 * 1000  # x seconds * 1000 miliseconds

CONTROL_POLYGON = [[39.34, 32.885], [39.36, 32.885], [39.37, 32.89], [39.35, 32.9075], [39.34, 32.905]]

# SAVE_PLOT = True
SAVE_PLOT = False
if SAVE_PLOT:
    if not os.path.exists('tracks'):
        os.makedirs('tracks')

SHOW_PLOT = True
# SHOW_PLOT = False
N_SECONDS_SLEEP = 0.1
# N_SECONDS_SLEEP = 0.1
# N_SECONDS_SLEEP = 0.005

# DISPLAY_IDS = True
DISPLAY_IDS = True

ONLY_CONTROLLED_DATA = True
# ONLY_CONTROLLED_DATA = False

# DISPLAY_OLD_FUSED_TRACKS_AS_POINTS = True
DISPLAY_OLD_FUSED_TRACKS_AS_POINTS = False

INDEX_ID = 0
INDEX_STATUS = 6
INDEX_LAT = 9
INDEX_LON = 10
INDEX_UPDATE_TIME = 12
INDEX_RADAR_ID = 13
MY_COUNT_ID = 21
MY_UNIQUE_ID = 22

MY_INDEX_ID = 0
MY_INDEX_LAT = 1
MY_INDEX_LON = 2
MY_INDEX_UPDATE_TIME = 3
MY_INDEX_RADAR_ID = 4
MY_INDEX_UNIQUE_ID = 5
MY_INDEX_COUNT_ID = 6

OLD_FUSED_TRACKS = np.array([[0, 0, 0, 0, 12]])


def process_data(data):
    # remove if any lat==0 or lon==0
    data = data[data[:, INDEX_LAT] != 0]
    data = data[data[:, INDEX_LON] != 0]

    # We could not read full update times in radar fusion code
    # we passed first 4 numbers of it, because they were always same, and the full time was too big
    # but the original datasets have full update times, so we add the 1585 (the 4 numbers we passed before)
    # to the output of radar fusion (just the update times)
    #    data[:, INDEX_UPDATE_TIME] += 1585000000000

    # sort the arrays depending on their update time columns
    data = data[np.argsort(data[:, INDEX_UPDATE_TIME])]

    return data


# data = process_data(data)


# set limits of the plot
min_x = 32.86942067
max_x = 32.93237458
min_y = 39.32927875
max_y = 39.38356196


def pointInPolygonTest(q, points):
    result = False

    pi = np.zeros(2)
    pj = np.zeros(2)

    # Jordan Curve Theorem
    i = 0
    j = len(points) - 1

    while i < len(points):
        pi[0] = points[i][1]
        pi[1] = points[i][0]
        pj[0] = points[j][1]
        pj[1] = points[j][0]

        if ((pi[1] > q[1]) != (pj[1] > q[1])) and (q[0] < (pj[0] - pi[0]) * (q[1] - pi[1]) / (pj[1] - pi[1]) + pi[0]):
            result = not result

        j = i
        i += 1
    return result


def remove_old_tracks(DATA, simulated_current_time):
    global REMOVE_OLD_TRACKS_TIME_CONSTANT
    return DATA[(simulated_current_time - DATA[:, MY_INDEX_UPDATE_TIME]) <= REMOVE_OLD_TRACKS_TIME_CONSTANT]


def update_data(DATA, new_id, new_lat, new_lon, new_time, new_radar_id, my_unique_id, my_count_id):
    global OLD_FUSED_TRACKS
    if ONLY_CONTROLLED_DATA:
        # check if new data is in control polygon
        if not pointInPolygonTest([new_lon, new_lat], CONTROL_POLYGON):
            return DATA

    already_exists_indexs = np.where(np.array(DATA[:, MY_INDEX_ID]) == new_id)
    count = already_exists_indexs[0].shape[0]

    if new_radar_id == 12 and DISPLAY_OLD_FUSED_TRACKS_AS_POINTS:
        OLD_FUSED_TRACKS = np.append(OLD_FUSED_TRACKS,
                                     [[new_id, new_lat, new_lon, new_time, new_radar_id, my_unique_id, my_count_id]],
                                     axis=0)

    # same id already exist, change it
    for i in range(count):
        already_exists_index = already_exists_indexs[0][i]

        if DATA[already_exists_index, MY_INDEX_RADAR_ID] == new_radar_id:
            DATA[already_exists_index, MY_INDEX_LAT] = new_lat
            DATA[already_exists_index, MY_INDEX_LON] = new_lon
            DATA[already_exists_index, MY_INDEX_UPDATE_TIME] = new_time
            DATA[already_exists_index, MY_INDEX_UNIQUE_ID] = my_unique_id
            DATA[already_exists_index, MY_INDEX_COUNT_ID] = my_count_id
            return DATA

    # id does not exist, append it
    DATA = np.append(DATA, [[new_id, new_lat, new_lon, new_time, new_radar_id, my_unique_id, my_count_id]], axis=0)

    return DATA


data_next_index = 0

# data_max_index = data.shape[0]


# initialize main array
# DATA = np.array([[data[0, INDEX_ID], data[0, INDEX_LAT], data[0, INDEX_LON], data[0, INDEX_UPDATE_TIME], data[0, INDEX_RADAR_ID]]])
DATA = np.array([[0, 0, 0, 0, 0, 0, 0]])
data_next_index = 1

# set plot size
plt.rcParams["figure.figsize"] = (9, 9)
# plt.rcParams["figure.figsize"] = (25,25)
satellite_img = plt.imread("golbek_satellite.png")
satellite_img /= 1.5
satellite_img_ext = [32.86935, 32.932, 39.329, 39.383]

angle_counter = 0

new_data_row_counter = 1

min_time = None

# main update and plot loop

# if (ONLY_CONTROLLED_DATA):
#    time_counter = 1585138764676
#    max_time = 1585139216676
# while (time_counter <= max_time):

while (1):

    while (1):
        try:
            new_data = np.loadtxt(open("data/radar_fused.csv", "rb"), delimiter=";", skiprows=new_data_row_counter,
                                  dtype=np.float64)
            if new_data.shape != (0,):
                new_data_row_counter += 1
                break
        except:
            pass

    #    data_new = np.array([[data[0, INDEX_ID], data[0, INDEX_LAT], data[0, INDEX_LON], data[0, INDEX_UPDATE_TIME], data[0, INDEX_RADAR_ID]]])

    if (len(new_data.shape) == 1):
        new_data = np.array([new_data])

    for row in new_data:
        if min_time is None:
            min_time = row[INDEX_UPDATE_TIME]
        DATA = update_data(DATA, row[INDEX_ID], row[INDEX_LAT], row[INDEX_LON], row[INDEX_UPDATE_TIME],
                           row[INDEX_RADAR_ID], row[MY_UNIQUE_ID], row[MY_COUNT_ID])

    time_counter = new_data[-1, INDEX_UPDATE_TIME]

    DATA = remove_old_tracks(DATA, time_counter)

    #    while (data_next_index < data_max_index and
    #           data[data_next_index, INDEX_UPDATE_TIME] <= time_counter):# and
    #        DATA = update_data(DATA, data[data_next_index, INDEX_ID], data[data_next_index, INDEX_LAT], data[data_next_index, INDEX_LON], data[data_next_index, INDEX_UPDATE_TIME], data[data_next_index, INDEX_RADAR_ID])
    #        data_next_index += 1

    # plot
    DATA_1 = DATA[DATA[:, MY_INDEX_RADAR_ID] == 1]
    DATA_2 = DATA[DATA[:, MY_INDEX_RADAR_ID] == 2]
    DATA_12 = DATA[DATA[:, MY_INDEX_RADAR_ID] == 12]

    if len(DATA_12) > 1:

        i = 0
        for data in DATA_12:
            for data2 in DATA_12:
                if data[0] != data2[0]:
                    lastRecordFirstNumber, lastRecordSecondNumber = str(data[MY_INDEX_UNIQUE_ID]).split("1001")
                    unique_id = data[MY_INDEX_UNIQUE_ID]
                    tempFirstNumber, tempLastNumber = str(data2[MY_INDEX_UNIQUE_ID]).split("1001")
                    if lastRecordFirstNumber == tempFirstNumber or lastRecordSecondNumber == tempLastNumber:
                        j = 0
                        for x in DATA_12:
                            if str(x[MY_INDEX_UNIQUE_ID]).split("1001")[0] == lastRecordFirstNumber or \
                                    str(x[MY_INDEX_UNIQUE_ID]).split("1001")[1] == lastRecordSecondNumber:
                                break
                            j += 1
                        DATA_12 = np.delete(DATA_12, i, axis=0)

    if len(DATA_1):
        plt.scatter(DATA_1[:, MY_INDEX_LON], DATA_1[:, MY_INDEX_LAT], c='#1f77b4', marker='|', s=100)
    if len(DATA_2):
        plt.scatter(DATA_2[:, MY_INDEX_LON], DATA_2[:, MY_INDEX_LAT], c='#d62728', marker='_', s=90)
    if len(DATA_12):
        plt.scatter(DATA_12[:, MY_INDEX_LON], DATA_12[:, MY_INDEX_LAT], c='#ADFF2F', marker='x', s=20)
    if len(OLD_FUSED_TRACKS):
        plt.scatter(OLD_FUSED_TRACKS[:, MY_INDEX_LON], OLD_FUSED_TRACKS[:, MY_INDEX_LAT], c='#DBE7BD', marker='.',
                    s=0.1)

    #    plt.scatter(DATA[:,MY_INDEX_LON], DATA[:,MY_INDEX_LAT], c='#1f77b4', marker='o')
    plt.imshow(satellite_img, zorder=0, extent=satellite_img_ext)

    if DISPLAY_IDS and len(DATA_12):
        #     display track ids
        for row in DATA_12:
            id_ = row[MY_INDEX_ID]
            radar_id = row[MY_INDEX_RADAR_ID]
            my_count = row[MY_INDEX_COUNT_ID]
            x = row[MY_INDEX_LON]
            y = row[MY_INDEX_LAT]

            move_x = math.sin((angle_counter * 30) % 360) / 8
            move_y = math.cos((angle_counter * 30) % 360) / 8
            angle_counter += 1

            x = (x - min_x) / (max_x - min_x) + move_x  # 0.005
            y = (y - min_y) / (max_y - min_y) + move_y  # 0.005

            if (row[MY_INDEX_RADAR_ID] == 1):
                color = '#1f77b4'
            elif (row[MY_INDEX_RADAR_ID] == 2):
                color = '#d62728'
            else:
                color = '#ADFF2F'

            plt.plot([row[MY_INDEX_LON], row[MY_INDEX_LON] + move_x / 17],
                     [row[MY_INDEX_LAT], row[MY_INDEX_LAT] + move_y / 17], color=color, linewidth=0.3)
            plt.text(x, y, "{}".format(str(id_) + "---" + str(my_count)), color=color,
                     transform=plt.gca().transAxes)

    plt.text(0.01, 1.05, "Time = {} ({})".format(int((time_counter - min_time) / 1000), time_counter),
             transform=plt.gca().transAxes)
    plt.xlim(min_x, max_x)
    plt.ylim(min_y, max_y)
    plt.grid(True)

    if SAVE_PLOT:
        plt.savefig('tracks/time{}.png'.format(time_counter))
    if SHOW_PLOT:
        #        if (time_counter < 1585138844676):
        plt.show(block=False)
        plt.pause(N_SECONDS_SLEEP)  # Pause for interval second
    #        else:
    #            plt.show()

    #    plt.pause(10000)
    # plt.waitforbuttonpress(0)

    # clear previous plot
    plt.clf()

plt.close('all')
print('Done')
