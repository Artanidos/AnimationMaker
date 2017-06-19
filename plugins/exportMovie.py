# exportMovie.py
# version: 1.1
# author: Olaf Japp (artanidos@gmail.com)
# copyright 2017, Olaf Japp

from PythonQt.QtCore import QFile, QDir, QProcess

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
		proc = QProcess()
		proc.start("ffmpeg -r " + str(fps) + " -safe 0 -f concat -i " + directory + "/list -b 4M -y " + output)
		proc.waitForFinished()
		print(proc.readAllStandardOutput())
		print(proc.readAllStandardError())

		statusbar.showMessage("Creating pallete from movie")
		proc = QProcess()
		proc.start("ffmpeg -i " + output + " -vf palettegen -y " + directory + "/temp.png")
		proc.waitForFinished()
		print(proc.readAllStandardOutput())
		print(proc.readAllStandardError())

		statusbar.showMessage("Converting movie to " + filename)
		proc = QProcess()
		proc.start("ffmpeg -r " + str(fps) + " -i " + output + " -i " + directory + "/temp.png -lavfi paletteuse -y " + filename)
		proc.waitForFinished()
		print(proc.readAllStandardOutput())
		print(proc.readAllStandardError())
	else:
		statusbar.showMessage("Creating movie " + filename)
		proc = QProcess()
		proc.start("ffmpeg -r " + str(fps) + " -safe 0 -f concat -i " + directory + "/list -b 4M -y " + filename)
		proc.waitForFinished()
		print(proc.readAllStandardOutput())
		print(proc.readAllStandardError())
	statusbar.showMessage("Ready")
	return
