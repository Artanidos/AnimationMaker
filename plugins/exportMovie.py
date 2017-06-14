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

def exportMovie(filename, directory):
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
	os.system("ffmpeg -safe 0 -f concat -i " + directory + "/list -c copy -y " + filename)
	return
