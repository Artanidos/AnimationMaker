from PythonQt.QtCore import QProcess, QFile, QDir
import os

def displayName():
	return "Movie"

def filter():
	return "Video format (*.mpg *.mp4 *.avi *.gif);;All Files (*)"

def title():
	return "Export Animation to Movie"

def type():
	return "EXPORT_MOVIE"

def exportMovie(filename, directory, fps):
	statusbar.showMessage("creating file " + filename)
	dir = QDir(directory)
	file = QFile(directory + "/list")
	if file.open(QIODevice.ReadWrite) == 0:
		QMessageBox.warning(0, "Error", "Unable to create file" + filename)
		return
	list = dir.entryList()
	for name in list:
		if name <> "." and name <> ".." and name <> "list":
			file.write("file '" + directory + "/" + name + "'\n")
	file.close()
	if filename.endswith(".gif"):
		output = directory + "/temp.mp4";
		os.system("ffmpeg -r " + str(fps) + " -safe 0 -f concat -i " + directory + "/list -c copy -y " + output)
		os.system("ffmpeg -i " + output + " -vf palettegen -y " + directory + "/temp.png")
		os.system("ffmpeg -r " + str(fps) + " -i " + output + " -i " + directory + "/temp.png -lavfi paletteuse -y " + filename)
	else:
		os.system("ffmpeg -r " + str(fps) + " -safe 0 -f concat -i " + directory + "/list -c copy -y " + filename)
	return
