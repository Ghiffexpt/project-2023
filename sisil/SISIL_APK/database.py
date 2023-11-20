import pandas


class Database():
    num_channels = 12

    def __init__(self, csv_file):
        self.csv_file = csv_file
        self.data = pandas.read_csv(csv_file)
        self.df = pandas.DataFrame(self.data)

    # read value of property of a channel
    def read(self, channel, property):
        return self.df.at[channel - 1, property]

    # write value for property of a channel
    def write(self, channel, property, value):
        self.df.at[channel - 1, property] = value

    # save to file
    def save(self):
        self.df.to_csv(self.csv_file, index=False)  # Save the DataFrame to the same CSV file
