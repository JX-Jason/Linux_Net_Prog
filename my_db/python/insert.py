import random
f = open("../insert.sql", "w")
for i in range(1,10000):
    f.write("insert db "+str(i)+" user" + str(i) + " " + str(random.randint(10,99)) + " " + str(random.randint(10000,99999)) + "@qq.com\n")
f.close()