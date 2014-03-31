import sys

# returns a range of step spaced decimals between start and stop
# like range() built-in but for decimals, too
def drange(start, stop, step):
    r = start
    while r < stop:
        yield r
        r += step

# writes test data between parameters to data.txt
# the data doesn't need to be realistic just within bounds - no angles of 
# 450 degrees, sensor will never return that
def write_test_data(maxLat = 40.0868, minLat = 40.001, stepLat = 0.001, maxLon = -105.1621, minLon = -105.2896, stepLon = 0.001):
    data = open("data.txt", 'w+')

    latlon = zip(drange(minLat, maxLat, stepLat), drange(minLon, maxLon, stepLon))
    length = len(latlon)
    for i in range(length):
        # write algor data
        data.write("{0};{1};{2};{3};{4};{5};{6};{7};{8};{9}\n".format((4*i)%360,
          10, (5*i)%360, 8, i%5, (4*i + 180)%360, 
          latlon[i][0], latlon[i][1], 45, 7))

        # write human data
        data.write("{0};{1};{2};{3};{4};{5};{6};{7};{8};{9}\n".format((4*i)%360,
          10, (5*i)%360, 8, -i%5, (4*i + 180)%360, latlon[length - i - 1][0], 
          latlon[length - i - 1][1], 225, 7))

    data.close()

if __name__ == '__main__':
    write_test_data()



