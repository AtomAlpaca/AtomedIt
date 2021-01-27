#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui -> mainEdit -> setPlainText(nofileTips.toUtf8());
    //定义编辑用字体 editFont
    //定义其他用字体 楷体
    //TODO:解决Linux上可能发生的字体缺失问题
    QFont editFont("Consolas");
    QFont elseFont("等线");
    setFont(elseFont);
    setWindowTitle("AtomedIt");
    //设置编辑字体为 editFont
    //将 Tab 设置为 4 格宽度
    //TODO:接受用户自定义 Tab 宽度
    ui -> mainEdit -> setFont(editFont);
    QFontMetrics metrics(ui -> mainEdit -> font());
    ui -> mainEdit -> setTabStopWidth(4 * metrics.width(' '));
    connect(ui -> openButton,&QPushButton::clicked,[=](){

        //是否已经在编辑文件
        if(!filePath.isEmpty())
        {
            //保存现有文件
            QFile oldFile(filePath);
            oldFile.open(QIODevice::ReadWrite);
            QByteArray context = ui -> mainEdit -> toPlainText().toUtf8();
            oldFile.write(context);
            oldFile.close();

            //关闭
            filePath = "";
        }

        //打开文件,并将文件路径赋值给 QString 类型变量 filePath
        //filePath 在 widget.h 中定义,主要是因为在这个文件中定义后进行赋值有时会崩......
        filePath = QFileDialog::getOpenFileName(this,"打开文件","C:\\");


        //检查一下用户到底打没打开文件
        if(filePath.isEmpty())
        {
            return;
        }


        //新建一个 QFile 对象 newFile
        QFile newFile(filePath);

        //以读写方式打开文件
        newFile.open(QIODevice::ReadWrite);

        //新建一个 QByteArray 对象,并赋值为文件的所有内容
        QByteArray context = newFile.readAll();
        ui -> mainEdit -> setPlainText(context);
        newFile.close();

    });

    connect(ui -> saveButton,&QPushButton::clicked,[=]()
    {
        if (filePath.isEmpty())
        {
            QMessageBox::warning(this,"啊偶,出错了","你似乎还没有打开文件......");
            return;
        }
        else
        {
            QFile newFile(filePath);
            newFile.open(QIODevice::ReadWrite);
            QByteArray context = ui -> mainEdit -> toPlainText().toUtf8();
            newFile.write(context);
            newFile.close();
            QMessageBox::information(this,"保存成功","你的文件已经成功保存!");
        }
    });

    connect(ui -> exitFileButton,&QPushButton::clicked,[=]()
    {
        if (filePath.isEmpty())
        {
            return;
        }
        else
        {
            QFile newFile(filePath);
            newFile.open(QIODevice::ReadWrite);
            QByteArray context = ui -> mainEdit -> toPlainText().toUtf8();
            newFile.write(context);
            newFile.close();
        }
        ui -> mainEdit -> setPlainText(nofileTips.toUtf8());
        filePath = "";
    });
    connect(ui -> exitButton,&QPushButton::clicked,[=]()
    {
        if(QMessageBox::question(this,"退出","要退出 AtomedIt 嘛? ",QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if(!filePath.isEmpty())
            {
                QFile newFile(filePath);
                newFile.open(QIODevice::ReadWrite);
                QByteArray context = ui -> mainEdit -> toPlainText().toUtf8();
                newFile.write(context);
                newFile.close();
            }
            QApplication::exit(0);
        }
    });
}

Widget::~Widget()
{
    delete ui;
}
