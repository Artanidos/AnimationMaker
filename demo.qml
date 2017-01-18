import QtQuick 2.3
import QtGraphicalEffects 1.0
import CrowdWare 1.0

Rectangle
{
    width: 1280
    height: 720
    color: "#0000FF"

    Ellipse
    {
        x: 400
        y: 100
        width: 100
        height: 100
        //border.width: 7
        //border.color: "green"
        color: "red"
    }

    Image
    {
        x: 250
        y: 250
        source: "qt.png"
    }

    Image
    {
        property int a
        id: image1
        x: 50
        y: 50
        source: "qt.png"
        transform: Rotation { origin.x: 30; origin.y: 30; axis { x: 0; y: 1; z: 0 } angle: image1.a }
    }

    Rectangle
    {
        id: rect2
        x: 50
        y: 100
        width: 100
        height: 100
        radius: width * 0.5
        smooth: true
        border.color: "black"
        border.width: 3
        opacity: 0.7

        RadialGradient
        {
            anchors.fill: parent
            source: parent
            horizontalOffset: -25
            verticalOffset: -25
            horizontalRadius: 50
            verticalRadius: 50
            gradient: Gradient
            {
                GradientStop { position: 0.0; color: "white" }
                GradientStop { position: 0.25; color: "#CDCDCD5B" }
                GradientStop { position: 1.0; color: "#CDCDCD5B" }
            }
        }
    }

    Text
    {
        id: text1
        text: "Hello World, this is my first video..."
        color: "#000000"
        font.bold: false
        font.pointSize: 30
        font.family: "Verdana"
        anchors.bottomMargin: 10
        anchors.rightMargin: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom

    }


    AnimationController
    {
        objectName: "animationController"
        animation: ParallelAnimation
        {
            SequentialAnimation
            {
                NumberAnimation { target: image1; property: "opacity"; duration: 600; from: 0; to: 1.0 }
                NumberAnimation { target: image1; property: "a"; duration: 600; from: 18; to: 72 }
            }
            ColorAnimation { target: text1; property: "color"; duration: 1200; from: "#000000"; to: "#FF0000" }
            PathAnimation
            {
                duration: 1200
                easing.type: Easing.InQuad
                target: image1
                orientation: PathAnimation.RightFirst
                anchorPoint: Qt.point(image1.width/2, image1.height/2)
                path: Path
                {
                    startX: 50; startY: 50

                    PathCubic
                    {
                        x: 100
                        y: 300
                        control1X: x; control1Y: 50
                        control2X: 50; control2Y: y
                    }
                }
            }
        }
    }
}
