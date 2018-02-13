import os
import sys
import re
rootdir = sys.argv[1]
pattern = re.compile('enum\\s+ColourIds\\s*{.*?}',re.DOTALL)
idMatch = re.compile('([a-zA-Z\\d_]+)\\s*=\\s*(0x[\\da-fA-F]+)')
with open('colorids.txt','w') as fout:
    colorIdList = []
    for root, subFolders, files in os.walk(rootdir):
        for file in files:
            if ('.h' in file):
                #print file
                with open(os.path.join(root, file), 'r') as fin:
                    text = fin.read()
                    for colorIDs in pattern.finditer(text):
                        idlist = colorIDs.group(0)
                        for color in idMatch.finditer(idlist):
                            foundId=color.group(2)+" = "+color.group(1)+";"+file
                            colorIdList.append(foundId)
    lastFile = ""
    for colorId in sorted(colorIdList):
        colorId = colorId.split(";")
        if lastFile != colorId[1]:
            fout.write(colorId[1]+"\n")
            lastFile = colorId[1]
        fout.write("\t"+colorId[0]+"\n")
