#include "disassemblerhighlighter.h"

DisassemblerHighlighter::DisassemblerHighlighter(QTextDocument *document) : QSyntaxHighlighter(document)
{
    this->_rgxdotted.setPattern("^[ \\t]+\\.\\w+");
}

void DisassemblerHighlighter::highlightBlock(const QString &text)
{
    if(this->_word.isEmpty())
        return;

    this->highlightWords(text);
    this->highlightSeek(text);
    this->highlightAll(text, this->_rgxdotted, this->_dottedcolor);
}

void DisassemblerHighlighter::highlightAll(const QString &text, const QRegularExpression &regex, const QColor &color)
{
    if(regex.pattern().isEmpty())
        return;

    QTextCharFormat charformat;
    charformat.setForeground(color);

    QRegularExpressionMatchIterator it = regex.globalMatch(text);

    while(it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        this->setFormat(match.capturedStart(), match.capturedLength(), charformat);
    }
}

void DisassemblerHighlighter::highlightWords(const QString &text)
{
    if(this->_word.isEmpty())
        return;

    QTextCharFormat charformat;
    charformat.setBackground(this->_highlightcolor);

    int idx = 0;

    while(idx < text.length())
    {
        idx = text.indexOf(this->_word, idx + this->_word.length());

        if(idx == -1)
            break;

        this->setFormat(idx, this->_word.length(), charformat);
    }
}

void DisassemblerHighlighter::highlightSeek(const QString &text)
{
    if(this->_rgxaddress.pattern().isEmpty())
        return;

    QTextCharFormat charformat;
    charformat.setForeground(this->_seekcolor);

    QRegularExpressionMatch match = this->_rgxaddress.match(text);

    if(!match.hasMatch())
        return;

    this->setFormat(match.capturedStart(), match.capturedLength(), charformat);
}

void DisassemblerHighlighter::setHighlightColor(const QColor &color)
{
    this->_highlightcolor = color;
}

void DisassemblerHighlighter::setDottedColor(const QColor &color)
{
    this->_dottedcolor = color;
}

void DisassemblerHighlighter::setSeekColor(const QColor &color)
{
    this->_seekcolor = color;
}

void DisassemblerHighlighter::highlight(const QString &word, const QString& currentaddress, const QTextBlock& block)
{
    this->_word = word;

    if(currentaddress.isEmpty())
        this->_rgxaddress.setPattern(QString());
    else
        this->_rgxaddress.setPattern("^[^\\:]+\\:" + currentaddress);

    this->rehighlightBlock(block);
}
