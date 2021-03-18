from os import system

for i in range(1, 13):
    system("./tlb {} 25 >> data.txt".format(1<<i))
system("echo >> data.txt")

try:
    import matplotlib.pyplot as plt
    import numpy as np

except:
    print( '''
    You need to install a library to generate the plot.
    The data is in a file called data.txt in the working directory.
    Each column is the time in ns for accessing 2^col_num pages
    ''')
    raise
    quit()

fig, ax = plt.subplots()

data = np.loadtxt("data.txt")
avg = data.mean(axis=0)

ax.plot(avg, marker=".")
x_ticks = range(0,13)
x_labels = [2<<i for i in x_ticks]

ax.set_xticks(x_ticks)
ax.set_xticklabels(x_labels)
ax.set_xlabel("Number of Pages")
ax.set_ylabel("Average Access Time (ns)")
plt.show()

fig.savefig("graph.png")
