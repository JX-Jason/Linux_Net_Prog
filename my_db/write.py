import random
f = open("./read.sql", "w")
for i in range(1,3):#modify here to change the number of the sql command.
    f.write("select * from db where id="+str(random.randint(0,999))+";\n")
    f.write("delete from db where id="+str(random.randint(0,999))+";\n")
    f.write("insert db "+str(random.randint(10000,99999))+" user" + str(i) + " " + str(random.randint(10,99)) + " " + str(random.randint(10000,99999)) + "@qq.com\n")
    f.write("update db user" + str(i) + " " + str(random.randint(10,99)) + " " + str(random.randint(10000,99999)) + "@qq.com where id=" + str(random.randint(10,99)) + ";\n")
f.close()