/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtVersit module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qversitwriter.h"
#include "qversitwriter_p.h"

#include <QtCore/qbuffer.h>
#include <QtCore/qtextcodec.h>

#include "qversitutils_p.h"

QT_BEGIN_NAMESPACE_VERSIT

/*!
  \class QVersitWriter
  \brief The QVersitWriter class writes Versit documents such as vCards to a device.
  \ingroup versit
  \inmodule QtVersit

  QVersitWriter converts a QVersitDocument into its textual representation.
  QVersitWriter supports writing to an abstract I/O device
  which can be for example a file or a memory buffer.
  The writing can be done asynchronously and the waitForFinished()
  function can be used to implement a blocking write.

  The serialization of the document is done in accordance with the type of the QVersitDocument
  being written.  The value of each QVersitProperty is encoded according to the type of object:
  \list
  \li \l{QString}{QStrings} are serialized verbatim, unless the default codec of the writer cannot
  encode the string: in this case, UTF-8 is used to encode it (and the CHARSET parameter added to
  the property, as per the vCard 2.1 specification).  If the document type is vCard 2.1,
  quoted-printable encoding may also be performed.
  \li \l{QByteArray}{QByteArrays} are assumed to be binary data and are serialized as base-64 encoded
  values.
  \li \l{QVersitDocument}{QVersitDocuments} are serialized as a nested document (eg. as per the
  AGENT property in vCard).
  \endlist

  \sa QVersitDocument, QVersitProperty
 */


/*!
 * \enum QVersitWriter::Error
 * This enum specifies an error that occurred during the most recent operation:
 * \value NoError The most recent operation was successful
 * \value UnspecifiedError The most recent operation failed for an undocumented reason
 * \value IOError The most recent operation failed because of a problem with the device
 * \value OutOfMemoryError The most recent operation failed due to running out of memory
 * \value NotReadyError The most recent operation failed because there is an operation in progress
 */

/*!
 * \enum QVersitWriter::State
 * Enumerates the various states that a reader may be in at any given time
 * \value InactiveState Write operation not yet started
 * \value ActiveState Write operation started, not yet finished
 * \value CanceledState Write operation is finished due to cancelation
 * \value FinishedState Write operation successfully completed
 */

/*!
 * \fn QVersitWriter::stateChanged(QVersitWriter::State state)
 * The signal is emitted by the writer when its state has changed (eg. when it has finished
 * writing to the device).
 * \a state is the new state of the writer.
 */

/*! Constructs a new writer. */
QVersitWriter::QVersitWriter() : d(new QVersitWriterPrivate)
{
    d->init(this);
}

/*! Constructs a new writer that writes to \a outputDevice. */
QVersitWriter::QVersitWriter(QIODevice *outputDevice) : d(new QVersitWriterPrivate)
{
    d->init(this);
    d->mIoDevice = outputDevice;
}

/*! Constructs a new writer that appends to \a outputBytes. */
QVersitWriter::QVersitWriter(QByteArray *outputBytes) : d(new QVersitWriterPrivate)
{
    d->init(this);
    d->mOutputBytes.reset(new QBuffer);
    d->mOutputBytes->setBuffer(outputBytes);
    d->mOutputBytes->open(QIODevice::WriteOnly);
    d->mIoDevice = d->mOutputBytes.data();
}

/*!
 * Frees the memory used by the writer.
 * Waits until a pending asynchronous writing has been completed.
 */
QVersitWriter::~QVersitWriter()
{
    d->wait();
    delete d;
}

/*!
 * Sets the device used for writing to \a device.
 * Does not take ownership of the device.
 */
void QVersitWriter::setDevice(QIODevice* device)
{
    d->mOutputBytes.reset(0);
    d->mIoDevice = device;
}

/*!
 * Returns the device used for writing, or 0 if no device has been set.
 */
QIODevice* QVersitWriter::device() const
{
    if (d->mOutputBytes.isNull())
        return d->mIoDevice;
    else
        return 0;
}

/*!
 * Sets the default codec for the writer to use for writing the entire output.
 *
 * If \a codec is NULL, the writer uses the codec according to the specification prescribed default.
 * (for vCard 2.1, ASCII; for vCard 3.0, UTF-8).
 */
void QVersitWriter::setDefaultCodec(QTextCodec *codec)
{
    d->mDefaultCodec = codec;
}

/*!
 * Returns the document's codec.
 */
QTextCodec* QVersitWriter::defaultCodec() const
{
    return d->mDefaultCodec;
}

/*!
 * Returns the state of the writer.
 */
QVersitWriter::State QVersitWriter::state() const
{
    return d->state();
}

/*!
 * Returns the error encountered by the last operation.
 */
QVersitWriter::Error QVersitWriter::error() const
{
    return d->error();
}

/*!
 * Starts writing \a input to device() asynchronously.  The serialization format is determined based
 * on the contents of the input documents.
 *
 * Returns false if the output device has not been set or opened or if there is another asynchronous
 * write operation already pending.  Signal \l stateChanged() is emitted with parameter
 * FinishedState when the writing has finished.
 *
 * The device must be already open.  The client is responsible for closing it when finished.
 */
bool QVersitWriter::startWriting(const QList<QVersitDocument>& input)
{
    return startWriting(input, QVersitDocument::InvalidType);
}

/*!
 * Starts writing \a input to device() asynchronously using the serialization format specified by \a
 * type.  If \a type is QVersitDocument::InvalidType, the format will be determined based on the
 * contents of the input documents.
 *
 * Returns false if the output device has not been set or opened or if there is another asynchronous
 * write operation already pending.  Signal \l stateChanged() is emitted with parameter
 * FinishedState when the writing has finished.
 *
 * The device must be already open.  The client is responsible for closing it when finished.
 */
bool QVersitWriter::startWriting(const QList<QVersitDocument>& input, QVersitDocument::VersitType type)
{
    d->mInput = input;
    if (d->state() == ActiveState || d->isRunning()) {
        d->setError(QVersitWriter::NotReadyError);
        return false;
    } else if (!d->mIoDevice || !d->mIoDevice->isWritable()) {
        d->setError(QVersitWriter::IOError);
        return false;
    } else {
        d->setState(ActiveState);
        d->setError(NoError);
        d->setDocumentType(type);
        d->start();
        return true;
    }
}

/*!
 * Starts writing \a input to device() asynchronously.  The serialization format is determined based
 * on the contents of the input documents.
 *
 * Returns false if the output device has not been set or opened or if there is another asynchronous
 * write operation already pending.  Signal \l stateChanged() is emitted with parameter
 * FinishedState when the writing has finished.
 *
 * The device must be already open.  The client is responsible for closing it when finished.
 */
bool QVersitWriter::startWriting(const QVersitDocument& input)
{
    return startWriting(QList<QVersitDocument>() << input, QVersitDocument::InvalidType);
}

/*!
 * Starts writing \a input to device() asynchronously using the serialization format specified by \a
 * type.  If \a type is QVersitDocument::InvalidType, the format will be determined based on the
 * contents of the input documents.
 *
 * Returns false if the output device has not been set or opened or if there is another asynchronous
 * write operation already pending.  Signal \l stateChanged() is emitted with parameter
 * FinishedState when the writing has finished.
 *
 * The device must be already open.  The client is responsible for closing it when finished.
 */
bool QVersitWriter::startWriting(const QVersitDocument& input, QVersitDocument::VersitType type)
{
    return startWriting(QList<QVersitDocument>() << input, type);
}

/*!
 * Attempts to asynchronously cancel the write request.
 */
void QVersitWriter::cancel()
{
    d->setCanceling(true);
}

/*!
 * If the state is ActiveState, blocks until the writer has finished writing or \a msec milliseconds
 * has elapsed, returning true if it successfully finishes or is cancelled by the user.
 * If \a msec is negative or zero, the function blocks until the writer has finished, regardless of
 * how long it takes.
 * If the state is FinishedState, returns true immediately.
 * Otherwise, returns false immediately.
 */
bool QVersitWriter::waitForFinished(int msec)
{
    State state = d->state();
    if (state != InactiveState) {
        if (msec <= 0)
            return d->wait(ULONG_MAX);
        else
            return d->wait(msec);
    } else {
        return false;
    }
}

QT_END_NAMESPACE_VERSIT

#include "moc_qversitwriter.cpp"
