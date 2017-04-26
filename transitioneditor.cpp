/****************************************************************************
** Copyright (C) 2016 Olaf Japp
**
** This file is part of AnimationMaker.
**
**  AnimationMaker is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  AnimationMaker is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with AnimationMaker.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "transitioneditor.h"
#include "expander.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QEasingCurve>

TransitionEditor::TransitionEditor()
{
    m_linear = QPixmap(":/images/linear.png");
    m_inquad = QPixmap(":/images/inquad.png");
    m_incubiq = QPixmap(":/images/incubiq.png");
    m_inquart = QPixmap(":/images/inquart.png");
    m_inquint = QPixmap(":/images/inquint.png");
    m_insine = QPixmap(":/images/insine.png");
    m_inexpo = QPixmap(":/images/inexpo.png");
    m_incirq = QPixmap(":/images/incirq.png");
    m_inback = QPixmap(":/images/inback.png");
    m_inelastic = QPixmap(":/images/inelastic.png");
    m_inbounce = QPixmap(":/images/inbounce.png");

    m_outquad = QPixmap(":/images/outquad.png");
    m_outcubiq = QPixmap(":/images/outcubiq.png");
    m_outquart = QPixmap(":/images/outquart.png");
    m_outquint = QPixmap(":/images/outquint.png");
    m_outsine = QPixmap(":/images/outsine.png");
    m_outexpo = QPixmap(":/images/outexpo.png");
    m_outcirq = QPixmap(":/images/outcirq.png");
    m_outback = QPixmap(":/images/outback.png");
    m_outelastic = QPixmap(":/images/outelastic.png");
    m_outbounce = QPixmap(":/images/outbounce.png");

    m_inoutquad = QPixmap(":/images/inoutquad.png");
    m_inoutcubiq = QPixmap(":/images/inoutcubiq.png");
    m_inoutquart = QPixmap(":/images/inoutquart.png");
    m_inoutquint = QPixmap(":/images/inoutquint.png");
    m_inoutsine = QPixmap(":/images/inoutsine.png");
    m_inoutexpo = QPixmap(":/images/inoutexpo.png");
    m_inoutcirq = QPixmap(":/images/inoutcirq.png");
    m_inoutback = QPixmap(":/images/inoutback.png");
    m_inoutelastic = QPixmap(":/images/inoutelastic.png");
    m_inoutbounce = QPixmap(":/images/inoutbounce.png");

    m_outinquad = QPixmap(":/images/outinquad.png");
    m_outincubiq = QPixmap(":/images/outincubiq.png");
    m_outinquart = QPixmap(":/images/outinquart.png");
    m_outinquint = QPixmap(":/images/outinquint.png");
    m_outinsine = QPixmap(":/images/outinsine.png");
    m_outinexpo = QPixmap(":/images/outinexpo.png");
    m_outincirq = QPixmap(":/images/outincirq.png");
    m_outinback = QPixmap(":/images/outinback.png");
    m_outinelastic = QPixmap(":/images/outinelastic.png");
    m_outinbounce = QPixmap(":/images/outinbounce.png");


    QVBoxLayout *vbox = new QVBoxLayout();
    Expander *exp = new Expander("Transition");
    QGridLayout *layout = new QGridLayout();
    m_curve = new QLabel();
    m_easing = new QListWidget();
    m_easing->setFixedHeight(175);
    m_easing->addItem("Linear");
    m_easing->addItem("Ease In");
    m_easing->addItem("Ease Out");
    m_easing->addItem("Ease In Out");
    m_easing->addItem("Ease Out In");
    m_type = new QListWidget();
    m_type->setFixedHeight(175);
    m_type->addItem("Quad");
    m_type->addItem("Cubiq");
    m_type->addItem("Quart");
    m_type->addItem("Quint");
    m_type->addItem("Sine");
    m_type->addItem("Expo");
    m_type->addItem("Circ");
    m_type->addItem("Back");
    m_type->addItem("Elastic");
    m_type->addItem("Bounce");
    layout->addWidget(m_easing, 0, 0, 3, 1);
    layout->addWidget(m_type, 0, 1, 3, 1);
    layout->addWidget(m_curve, 0, 2);
    exp->addLayout(layout);
    vbox->addWidget(exp);
    vbox->addStretch();
    this->setLayout(vbox);
    listRowChanged(0);

    connect(m_easing, SIGNAL(currentRowChanged(int)), this, SLOT(listRowChanged(int)));
    connect(m_type, SIGNAL(currentRowChanged(int)), this, SLOT(setCurve()));
}

void TransitionEditor::setKeyframe(KeyFrame *frame)
{
    m_frame = frame;
    switch(m_frame->easing())
    {
        case QEasingCurve::Linear:
            m_easing->setCurrentRow(0);
            break;
        case QEasingCurve::InQuad:
            m_easing->setCurrentRow(1);
            m_type->setCurrentRow(0);
            break;
        case QEasingCurve::OutQuad:
            m_easing->setCurrentRow(2);
            m_type->setCurrentRow(0);
            break;
        case QEasingCurve::InOutQuad:
            m_easing->setCurrentRow(3);
            m_type->setCurrentRow(0);
            break;
        case QEasingCurve::OutInQuad:
            m_easing->setCurrentRow(4);
            m_type->setCurrentRow(0);
            break;
        case QEasingCurve::InCubic:
            m_easing->setCurrentRow(1);
            m_type->setCurrentRow(1);
            break;
        case QEasingCurve::OutCubic:
            m_easing->setCurrentRow(2);
            m_type->setCurrentRow(1);
            break;
        case QEasingCurve::InOutCubic:
            m_easing->setCurrentRow(3);
            m_type->setCurrentRow(1);
            break;
        case QEasingCurve::OutInCubic:
            m_easing->setCurrentRow(4);
            m_type->setCurrentRow(1);
            break;
        case QEasingCurve::InQuart:
            m_easing->setCurrentRow(1);
            m_type->setCurrentRow(2);
            break;
        case QEasingCurve::OutQuart:
            m_easing->setCurrentRow(2);
            m_type->setCurrentRow(2);
            break;
        case QEasingCurve::InOutQuart:
            m_easing->setCurrentRow(3);
            m_type->setCurrentRow(2);
            break;
        case QEasingCurve::OutInQuart:
            m_easing->setCurrentRow(4);
            m_type->setCurrentRow(2);
            break;
        case QEasingCurve::InQuint:
            m_easing->setCurrentRow(1);
            m_type->setCurrentRow(3);
            break;
        case QEasingCurve::OutQuint:
            m_easing->setCurrentRow(2);
            m_type->setCurrentRow(3);
            break;
        case QEasingCurve::InOutQuint:
            m_easing->setCurrentRow(3);
            m_type->setCurrentRow(3);
            break;
        case QEasingCurve::OutInQuint:
            m_easing->setCurrentRow(4);
            m_type->setCurrentRow(3);
            break;
        case QEasingCurve::InSine:
            m_easing->setCurrentRow(1);
            m_type->setCurrentRow(4);
            break;
        case QEasingCurve::OutSine:
            m_easing->setCurrentRow(2);
            m_type->setCurrentRow(4);
            break;
        case QEasingCurve::InOutSine:
            m_easing->setCurrentRow(3);
            m_type->setCurrentRow(4);
            break;
        case QEasingCurve::OutInSine:
            m_easing->setCurrentRow(4);
            m_type->setCurrentRow(4);
            break;
        case QEasingCurve::InExpo:
            m_easing->setCurrentRow(1);
            m_type->setCurrentRow(5);
            break;
        case QEasingCurve::OutExpo:
            m_easing->setCurrentRow(2);
            m_type->setCurrentRow(5);
            break;
        case QEasingCurve::InOutExpo:
            m_easing->setCurrentRow(3);
            m_type->setCurrentRow(5);
            break;
        case QEasingCurve::OutInExpo:
            m_easing->setCurrentRow(4);
            m_type->setCurrentRow(5);
            break;
        case QEasingCurve::InCirc:
            m_easing->setCurrentRow(1);
            m_type->setCurrentRow(6);
            break;
        case QEasingCurve::OutCirc:
            m_easing->setCurrentRow(2);
            m_type->setCurrentRow(6);
            break;
        case QEasingCurve::InOutCirc:
            m_easing->setCurrentRow(3);
            m_type->setCurrentRow(6);
            break;
        case QEasingCurve::OutInCirc:
            m_easing->setCurrentRow(4);
            m_type->setCurrentRow(6);
            break;
        case QEasingCurve::InElastic:
            m_easing->setCurrentRow(1);
            m_type->setCurrentRow(7);
            break;
        case QEasingCurve::OutElastic:
            m_easing->setCurrentRow(2);
            m_type->setCurrentRow(7);
            break;
        case QEasingCurve::InOutElastic:
            m_easing->setCurrentRow(3);
            m_type->setCurrentRow(7);
            break;
        case QEasingCurve::OutInElastic:
            m_easing->setCurrentRow(4);
            m_type->setCurrentRow(7);
            break;
        case QEasingCurve::InBack:
            m_easing->setCurrentRow(1);
            m_type->setCurrentRow(8);
            break;
        case QEasingCurve::OutBack:
            m_easing->setCurrentRow(2);
            m_type->setCurrentRow(8);
            break;
        case QEasingCurve::InOutBack:
            m_easing->setCurrentRow(3);
            m_type->setCurrentRow(8);
            break;
        case QEasingCurve::OutInBack:
            m_easing->setCurrentRow(4);
            m_type->setCurrentRow(8);
            break;
        case QEasingCurve::InBounce:
            m_easing->setCurrentRow(1);
            m_type->setCurrentRow(9);
            break;
        case QEasingCurve::OutBounce:
            m_easing->setCurrentRow(2);
            m_type->setCurrentRow(9);
            break;
        case QEasingCurve::InOutBounce:
            m_easing->setCurrentRow(3);
            m_type->setCurrentRow(9);
            break;
        case QEasingCurve::OutInBounce:
            m_easing->setCurrentRow(4);
            m_type->setCurrentRow(9);
            break;
    }
}

void TransitionEditor::listRowChanged(int row)
{
    for(int i=0;i<m_type->count();i++)
        m_type->setRowHidden(i, row == 0);

    setCurve();
}

void TransitionEditor::setCurve()
{
    int easing = m_easing->currentRow();
    int type = m_type->currentRow();

    switch(easing)
    {
        case 0: // Linear
            m_curve->setPixmap(m_linear);
            m_frame->setEasing(QEasingCurve::Linear);
            break;
        case 1: // Ease In
        {
            switch(type)
            {
                case 0: // Quad
                    m_curve->setPixmap(m_inquad);
                    m_frame->setEasing(QEasingCurve::InQuad);
                    break;
                case 1: // Cubiq
                    m_curve->setPixmap(m_incubiq);
                    m_frame->setEasing(QEasingCurve::InCubic);
                    break;
                case 2: // Quart
                    m_curve->setPixmap(m_inquart);
                    m_frame->setEasing(QEasingCurve::InQuart);
                    break;
                case 3: // Quint
                    m_curve->setPixmap(m_inquint);
                    m_frame->setEasing(QEasingCurve::InQuint);
                    break;
                case 4: // Sine
                    m_curve->setPixmap(m_insine);
                    m_frame->setEasing(QEasingCurve::InSine);
                    break;
                case 5: // Expo
                    m_curve->setPixmap(m_inexpo);
                    m_frame->setEasing(QEasingCurve::InExpo);
                    break;
                case 6: // Cirq
                    m_curve->setPixmap(m_incirq);
                    m_frame->setEasing(QEasingCurve::InCirc);
                    break;
                case 7: // Back
                    m_curve->setPixmap(m_inback);
                    m_frame->setEasing(QEasingCurve::InBack);
                    break;
                case 8: // Elastic
                    m_curve->setPixmap(m_inelastic);
                    m_frame->setEasing(QEasingCurve::InElastic);
                    break;
                case 9:
                    m_curve->setPixmap(m_inbounce);
                    m_frame->setEasing(QEasingCurve::InBounce);
                    break;
            }
            break;
        }
        case 2: // Ease Out
        {
            switch(type)
            {
                case 0: // Quad
                    m_curve->setPixmap(m_outquad);
                    m_frame->setEasing(QEasingCurve::OutQuad);
                    break;
                case 1: // Cubiq
                    m_curve->setPixmap(m_outcubiq);
                    m_frame->setEasing(QEasingCurve::OutCubic);
                    break;
                case 2: // Quart
                    m_curve->setPixmap(m_outquart);
                    m_frame->setEasing(QEasingCurve::OutQuart);
                    break;
                case 3: // Quint
                    m_curve->setPixmap(m_outquint);
                    m_frame->setEasing(QEasingCurve::OutQuint);
                    break;
                case 4: // Sine
                    m_curve->setPixmap(m_outsine);
                    m_frame->setEasing(QEasingCurve::OutSine);
                    break;
                case 5: // Expo
                    m_curve->setPixmap(m_outexpo);
                    m_frame->setEasing(QEasingCurve::OutExpo);
                    break;
                case 6: // Cirq
                    m_curve->setPixmap(m_outcirq);
                    m_frame->setEasing(QEasingCurve::OutCirc);
                    break;
                case 7: // Back
                    m_curve->setPixmap(m_outback);
                    m_frame->setEasing(QEasingCurve::OutBack);
                    break;
                case 8: // Elastic
                    m_curve->setPixmap(m_outelastic);
                    m_frame->setEasing(QEasingCurve::OutElastic);
                    break;
                case 9:
                    m_curve->setPixmap(m_outbounce);
                    m_frame->setEasing(QEasingCurve::OutBounce);
                    break;
            }
            break;
        }
        case 3: // Ease In Out
        {
            switch(type)
            {
                case 0: // Quad
                    m_curve->setPixmap(m_inoutquad);
                    m_frame->setEasing(QEasingCurve::InOutQuad);
                    break;
                case 1: // Cubiq
                    m_curve->setPixmap(m_inoutcubiq);
                    m_frame->setEasing(QEasingCurve::InOutCubic);
                    break;
                case 2: // Quart
                    m_curve->setPixmap(m_inoutquart);
                    m_frame->setEasing(QEasingCurve::InOutQuart);
                    break;
                case 3: // Quint
                    m_curve->setPixmap(m_inoutquint);
                    m_frame->setEasing(QEasingCurve::InOutQuint);
                    break;
                case 4: // Sine
                    m_curve->setPixmap(m_inoutsine);
                    m_frame->setEasing(QEasingCurve::InOutSine);
                    break;
                case 5: // Expo
                    m_curve->setPixmap(m_inoutexpo);
                    m_frame->setEasing(QEasingCurve::InOutExpo);
                    break;
                case 6: // Cirq
                    m_curve->setPixmap(m_inoutcirq);
                    m_frame->setEasing(QEasingCurve::InOutCirc);
                    break;
                case 7: // Back
                    m_curve->setPixmap(m_inoutback);
                    m_frame->setEasing(QEasingCurve::InOutBack);
                    break;
                case 8: // Elastic
                    m_curve->setPixmap(m_inoutelastic);
                    m_frame->setEasing(QEasingCurve::InOutElastic);
                    break;
                case 9:
                    m_curve->setPixmap(m_inoutbounce);
                    m_frame->setEasing(QEasingCurve::InOutBounce);
                    break;
            }
            break;
        }
        case 4: // Ease Out In
        {
            switch(type)
            {
                case 0: // Quad
                    m_curve->setPixmap(m_outinquad);
                    m_frame->setEasing(QEasingCurve::OutInQuad);
                    break;
                case 1: // Cubiq
                    m_curve->setPixmap(m_outincubiq);
                    m_frame->setEasing(QEasingCurve::OutInCubic);
                    break;
                case 2: // Quart
                    m_curve->setPixmap(m_outinquart);
                    m_frame->setEasing(QEasingCurve::OutInQuart);
                    break;
                case 3: // Quint
                    m_curve->setPixmap(m_outinquint);
                    m_frame->setEasing(QEasingCurve::OutInQuint);
                    break;
                case 4: // Sine
                    m_curve->setPixmap(m_outinsine);
                    m_frame->setEasing(QEasingCurve::OutInSine);
                    break;
                case 5: // Expo
                    m_curve->setPixmap(m_outinexpo);
                    m_frame->setEasing(QEasingCurve::OutInExpo);
                    break;
                case 6: // Cirq
                    m_curve->setPixmap(m_outincirq);
                    m_frame->setEasing(QEasingCurve::OutInCirc);
                    break;
                case 7: // Back
                    m_curve->setPixmap(m_outinback);
                    m_frame->setEasing(QEasingCurve::OutInBack);
                    break;
                case 8: // Elastic
                    m_curve->setPixmap(m_outinelastic);
                    m_frame->setEasing(QEasingCurve::OutInElastic);
                    break;
                case 9:
                    m_curve->setPixmap(m_outinbounce);
                    m_frame->setEasing(QEasingCurve::OutInBounce);
                    break;
            }
            break;
        }
    }
}
