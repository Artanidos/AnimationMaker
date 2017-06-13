from PythonQt.QtXml import QDomDocument
from PythonQt.QtCore import QFile, QIODevice, QTextStream
from PythonQt.animationmaker import *

def displayName():
	return "XML"

def filter():
	return "XML format (*.xml);;All Files (*)"

def title():
	return "Export Animation to XML"

def type():
	return "EXPORT_META"

def writeKeyframes(item, ele, doc):
	for key, value in item.keyframes().iteritems():
		keyframes = doc.createElement("Keyframes")
		keyframes.setAttribute("property", key)
		ele.appendChild(keyframes)
		frame = value
		while frame <> None:
			keyframe = doc.createElement("Keyframe")
			keyframe.setAttribute("time", frame.time())
			keyframe.setAttribute("value", frame.value())
			keyframe.setAttribute("easing", frame.easing())
			keyframes.appendChild(keyframe)
			frame = frame.next()

def exportMeta(filename, exportAll, animation):
	statusbar.showMessage("Writing to file " + filename)
	xmlfile = QFile(filename)
	if xmlfile.open(QIODevice.WriteOnly) == 0:
		QMessageBox.warning(0, "Error", "Unable to open file")
		return
	
	doc = QDomDocument()
	if exportAll:
		root = doc.createElement("Animation")
		root.setAttribute("fps", animation.fps())
		root.setAttribute("width", animation.width())
		root.setAttribute("height", animation.height())
		doc.appendChild(root)
	else:
		root = doc.createElement("AnimationItems")
		doc.appendChild(root)

	for item in animation.items():
		if isinstance(item, Rectangle):
			ele = doc.createElement("Rectangle")
			ele.setAttribute("id", item.id())
			ele.setAttribute("left", "%.9f" % item.left())
			ele.setAttribute("top", "%.9f" % item.top())
			ele.setAttribute("opacity", item.opacity())
			ele.setAttribute("width", "%.9f" % item.width())
			ele.setAttribute("height", "%.9f" % item.height())
			ele.setAttribute("pen", item.pen())
			ele.setAttribute("brush", item.brush())
			root.appendChild(ele)
			writeKeyframes(item, ele, doc)
		elif isinstance(item, Ellipse):
			ele = doc.createElement("Ellipse")
			ele.setAttribute("id", item.id())
			ele.setAttribute("left", "%.9f" % item.left())
			ele.setAttribute("top", "%.9f" % item.top())
			ele.setAttribute("opacity", item.opacity())
			ele.setAttribute("width", "%.9f" % item.width())
			ele.setAttribute("height", "%.9f" % item.height())
			ele.setAttribute("pen", item.pen())
			ele.setAttribute("brush", item.brush())
			root.appendChild(ele)
			writeKeyframes(item, ele, doc)
		elif isinstance(item, Text):
			ele = doc.createElement("Text")
			ele.setAttribute("id", item.id())
			ele.setAttribute("left", "%.9f" % item.left())
			ele.setAttribute("top", "%.9f" % item.top())
			ele.setAttribute("opacity", item.opacity())
			ele.setAttribute("xscale", "%.9f" % item.xscale())
			ele.setAttribute("yscale", "%.9f" % item.yscale())
			ele.setAttribute("text", item.text())
			ele.setAttribute("textcolor", item.textcolor())
			ele.setAttribute("font-family", item.fontFamily())			
			ele.setAttribute("font-size", item.fontSize())
			ele.setAttribute("font-style", item.fontStyle())
			root.appendChild(ele)
			writeKeyframes(item, ele, doc)
		elif isinstance(item, Bitmap):
			ele = doc.createElement("Bitmap")
			ele.setAttribute("id", item.id())
			ele.setAttribute("left", "%.9f" % item.left())
			ele.setAttribute("top", "%.9f" % item.top())
			ele.setAttribute("opacity", item.opacity())
			ele.setAttribute("width", "%.9f" % item.width())
			ele.setAttribute("height", "%.9f" % item.height())
			ele.appendChild(doc.createCDATASection(item.image()))
			root.appendChild(ele)
			writeKeyframes(item, ele, doc)
		elif isinstance(item, Vectorgraphic):
			ele = doc.createElement("Vectorgraphic")
			ele.setAttribute("id", item.id())
			ele.setAttribute("left", "%.9f" % item.left())
			ele.setAttribute("top", "%.9f" % item.top())
			ele.setAttribute("opacity", item.opacity())
			ele.setAttribute("xscale", "%.9f" % item.xscale())
			ele.setAttribute("yscale", "%.9f" % item.yscale())
			ele.appendChild(doc.createCDATASection(item.data()))
			root.appendChild(ele)
			writeKeyframes(item, ele, doc)

	stream = QTextStream(xmlfile)
	doc.save(stream, 0)
	xmlfile.close()
	statusbar.showMessage("Ready")
	return
