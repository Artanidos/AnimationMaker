import os

def exportMovie(filename, directory, fps):
	f = open(directory + '/list', 'w')
	lst = os.listdir(directory)
	lst.sort()
	for name in lst:
		if name.endswith(".png"):
			f.write("file '" + directory + "/" + name + "'\n")
	f.close()
	if filename.endswith(".gif"):
		output = directory + "/temp.mp4";
		os.system("ffmpeg -r " + str(fps) + " -safe 0 -f concat -i " + directory + "/list -b 4M -y " + output)
		os.system("ffmpeg -i " + output + " -vf palettegen -y " + directory + "/temp.png")
		os.system("ffmpeg -r " + str(fps) + " -i " + output + " -i " + directory + "/temp.png -lavfi paletteuse -y " + filename)
	else:
		os.system("ffmpeg -r " + str(fps) + " -safe 0 -f concat -i " + directory + "/list -b 4M -y " + filename)
	return
