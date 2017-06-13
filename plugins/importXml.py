from PythonQt.QtXml import QDomDocument
from PythonQt.QtCore import QFile, QIODevice, QTextStream
from PythonQt.animationmaker import *

def displayName():
	return "XML"

def filter():
	return "XML format (*.xml);;All Files (*)"

def title():
	return "Import Animation from XML"

def type():
	return "IMPORT_META"

def readKeyframes(ele, item):
	m_tempKeyFrame = None
	for i in range(0, ele.childNodes().count()):
		node = ele.childNodes().at(i)
		if node.nodeName() == "Keyframes":
			keyframes = node.toElement()
			for j in range(0, node.childNodes().count()):
				frameNode = node.childNodes().at(j)
				if frameNode.nodeName() == "Keyframe":
					keyframe = frameNode.toElement()
					key = KeyFrame()
					key.setTime(int(keyframe.attribute("time", "0")))
					key.setValue(keyframe.attribute("value"))
					key.setEasing(int(keyframe.attribute("easing", "-1")))
					if m_tempKeyFrame <> None:
						m_tempKeyFrame.setNext(key)
						key.setPrev(m_tempKeyFrame)
					else:
						item.insertKeyframe(keyframes.attribute("property"), key)
					m_tempKeyFrame = key

def importMeta(filename, animation):
	statusbar.showMessage("Reading from file " + filename)
	xmlfile = QFile(filename)
	if xmlfile.open(QIODevice.ReadOnly) == 0:
		QMessageBox.warning(0, "Error", "Unable to open file" + filename)
		return
	doc = QDomDocument()
	if doc.setContent(xmlfile) == 0:
		QMessageBox.warning(0, "Error", "Unable to read file " + filename)
		xmlfile.close()
		bar.showMessage("Unable to read file " + filename);
		return
	xmlfile.close()
	docElem = doc.documentElement()
	if docElem.nodeName() == "Animation":
		animation.reset()
		animation.setFps(int(docElem.attribute("fps", "24")))
		animation.setWidth(int(docElem.attribute("width", "1200")))
		animation.setHeight(int(docElem.attribute("height", "720")))
	for i in range(0, docElem.childNodes().count()):
		node = docElem.childNodes().at(i)
		ele = node.toElement()
		if node.nodeName() == "Rectangle":
			r = Rectangle()
			r.setId(ele.attribute("id", "Rectangle"))
			r.setLeft(float(ele.attribute("left", "0")))
			r.setTop(float(ele.attribute("top", "0")))
			r.setOpacity(int(ele.attribute("opacity", "100")))
			r.setWidth(float(ele.attribute("width", "50")))
			r.setHeight(float(ele.attribute("height", "50")))
			r.setPen(ele.attribute("pen", "#000000"))
			r.setBrush(ele.attribute("brush", "#0000FF"))
			animation.addItem(r)
			readKeyframes(ele, r)
		elif node.nodeName() == "Ellipse":
			e = Ellipse()
			e.setId(ele.attribute("id", "Ellipse"))
			e.setLeft(float(ele.attribute("left", "0")))
			e.setTop(float(ele.attribute("top", "0")))
			e.setOpacity(int(ele.attribute("opacity", "100")))
			e.setWidth(float(ele.attribute("width", "50")))
			e.setHeight(float(ele.attribute("height", "50")))
			e.setPen(ele.attribute("pen", "#000000"))
			e.setBrush(ele.attribute("brush", "#0000FF"))
			animation.addItem(e)
			readKeyframes(ele, e)
		elif node.nodeName() == "Text":
			t = Text()
			t.setId(ele.attribute("id", "Text"))
			t.setLeft(float(ele.attribute("left", "0")))
			t.setTop(float(ele.attribute("top", "0")))
			t.setOpacity(int(ele.attribute("opacity", "100")))
			t.setText(ele.attribute("text"))
			t.setTextcolor(ele.attribute("textcolor", "#000000"))
			t.setFontFamily(ele.attribute("font-family"))
			t.setFontSize(int(ele.attribute("font-size")))
			t.setFontStyle(ele.attribute("font-style"))
			t.setXscale(float(ele.attribute("xscale", "1")))
			t.setYscale(float(ele.attribute("yscale", "1")))
			animation.addItem(t)
			readKeyframes(ele, t)
		elif node.nodeName() == "Bitmap":
			b = Bitmap()
			b.setId(ele.attribute("id", "Text"))
			b.setLeft(float(ele.attribute("left", "0")))
			b.setTop(float(ele.attribute("top", "0")))
			b.setOpacity(int(ele.attribute("opacity", "100")))
			b.setWidth(float(ele.attribute("width", "50")))
			b.setHeight(float(ele.attribute("height", "50")))
			b.setImage(ele.attribute("id", "Text"))
			data = ele.firstChild()
			cdata = data.toCDATASection()
			b.setImage(cdata.data())
			animation.addItem(b)
			readKeyframes(ele, b)
		elif node.nodeName() == "Vectorgraphic":
			v = Vectorgraphic()
			v.setId(ele.attribute("id", "Text"))
			v.setLeft(float(ele.attribute("left", "0")))
			v.setTop(float(ele.attribute("top", "0")))
			v.setOpacity(int(ele.attribute("opacity", "100")))
			v.setXscale(float(ele.attribute("xscale", "1")))
			v.setYscale(float(ele.attribute("yscale", "1")))
			data = ele.firstChild()
			cdata = data.toCDATASection()
			v.setData(cdata.data())
			animation.addItem(v)
			readKeyframes(ele, v)
	statusbar.showMessage("Ready")
	return
