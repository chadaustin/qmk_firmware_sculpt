#!/usr/bin/python3

# Automatically makes the wacky LAYOUT stuff for sculpt.h

used = [
	"k4D", "k7C", "k7B", "k1B", "k4B", "k1A", "k1F", "k79", "k77", "k75", "k78", "k73", "k13", "k71", "k31", "k01", "k23",
    "k7D", "k0D", "k0C", "k2D", "k1D", "k7A", "k7F", "k09", "k07", "k05", "k04", "k15", "k74", "k08", "k03", "k21",
    "k0A", "k0B", "k1C", "k2B", "k2A", "k2F", "k19", "k29", "k27", "k25", "k24", "k14", "k17", "k38", "k51",
    "k2C", "k3D", "k4C", "k3B", "k3A", "k4A", "k49", "k39", "k37", "k35", "k34", "k45", "k33", "k18", "k11",
    "k5E", "k5D", "k5C", "k5B", "k5A", "k6A", "k69", "k59", "k57", "k55", "k44", "k52", "k63", "k53",
    "k6H", "k3F", "k4G", "k6B", "k68", "k46", "k43", "k60", "k48", "k64", "k61",
]

for r in range(0,8):
    line = "{"
    for c in range(0,18):
        if (c <= 9):
            place = "k%i%i" % (r, c)
        else:
            place = "k%i%s" % (r, chr(55 + c))
        if place not in used:
            place = "KC_NO"
        line = line + "%s, " % place
    line = line.strip(", ") + "}, \\"
    print(line)

