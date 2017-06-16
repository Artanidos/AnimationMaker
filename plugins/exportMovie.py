# exportMovie.py
# version: 1.0
# author: Olaf Japp (artanidos@gmail.com)
# copyright 2017, Olaf Japp

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
		statusbar.showMessage("Creating temporary movie " + output)
		os.system("ffmpeg -r " + str(fps) + " -safe 0 -f concat -i " + directory + "/list -b 4M -y " + output)
		statusbar.showMessage("Creating pallete from movie")
		os.system("ffmpeg -i " + output + " -vf palettegen -y " + directory + "/temp.png")
		statusbar.showMessage("Converting movie to " + filename)
		os.system("ffmpeg -r " + str(fps) + " -i " + output + " -i " + directory + "/temp.png -lavfi paletteuse -y " + filename)
	else:
		statusbar.showMessage("Creating movie " + filename)
		os.system("ffmpeg -r " + str(fps) + " -safe 0 -f concat -i " + directory + "/list -b 4M -y " + filename)
	statusbar.showMessage("Ready")
	return
