import pandas

dataframe = pandas.read_csv("data/radar_fused2.csv", sep=";", index_col=False)

# print(len(dataframe[dataframe['RadarId'] == 12]))

df_columns = dataframe.columns
delete_rows = []
for index, row in dataframe.iterrows():
    if row['RadarId'] == 12:
        delete_rows.append(index)

dataframe = dataframe.drop(delete_rows)
dataframe.to_csv('data/radar_fused2.csv', sep=';', index=False, index_label=False)
print(dataframe)
