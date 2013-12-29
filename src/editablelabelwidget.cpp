/*
    Copyright (C) 2013 by Maxim Biro <nurupo.contributions@gmail.com>
    
    This file is part of Tox Qt GUI.
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    
    See the COPYING file for more details.
*/

#include "editablelabelwidget.hpp"

#include <QVBoxLayout>

EditableLabelWidget::EditableLabelWidget(QWidget* parent) :
    QStackedWidget(parent)
{
    label = new ClickableLabel(this);

    connect(label, &ClickableLabel::clicked,            this, &EditableLabelWidget::onLabelClicked);

    lineEdit = new EscLineEdit(this);

    connect(lineEdit, &EscLineEdit::editingFinished,    this, &EditableLabelWidget::onLabelChangeSubmited);
    connect(lineEdit, &EscLineEdit::escPressed,         this, &EditableLabelWidget::onLabelChangeCancelled);

    connect(this, &EditableLabelWidget::currentChanged, this, &EditableLabelWidget::onCurrentChanged);

    addWidget(label);
    addWidget(lineEdit);

    setCurrentWidget(label);
}

void EditableLabelWidget::setText(const QString& text)
{
    label->setText(text);
    lineEdit->setText(text);
}

void EditableLabelWidget::addWidget(QWidget* w)
{
    w->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    QStackedWidget::addWidget(w);
}

void EditableLabelWidget::onCurrentChanged(int index)
{
    QWidget* w = widget(index);
    if (w == label) {
        lineEdit->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    } else {
        label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    }
    w->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    w->adjustSize();
    adjustSize();
}

void EditableLabelWidget::onLabelChangeSubmited()
{
    QString oldText = label->text();
    QString newText = lineEdit->text();
    lineEdit->clearFocus();
    setCurrentWidget(label);

    if (oldText != newText) {
        label->setText(newText);
        emit textChanged(newText, oldText);
    }
}

void EditableLabelWidget::onLabelChangeCancelled()
{
    // order of calls matters, since clearFocus() triggers EditableLabelWidget::onLabelChangeSubmited()
    lineEdit->setText(label->text());
    lineEdit->clearFocus();
    setCurrentWidget(label);
}

void EditableLabelWidget::onLabelClicked()
{
    setCurrentWidget(lineEdit);
    lineEdit->setFocus();
}
