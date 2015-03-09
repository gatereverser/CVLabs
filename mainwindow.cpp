//#include "mainwindow.h"
//#include "ui_mainwindow.h"

//MainWindow::MainWindow(QWidget *parent) :
//    QMainWindow(parent),
//    ui(new Ui::MainWindow)
//{
//    ui->setupUi(this);
//    scene = new QGraphicsScene();
//   ui->graphicsView->setScene(scene);
//   item = scene->addPixmap(QPixmap::fromImage(QImage()));
//}

//MainWindow::~MainWindow()
//{
//    delete item;
//    delete scene;
//    delete ui;
//}

//void MainWindow::on_LoadButton_clicked()
//{
//   QFileDialog dialog(this);
//    dialog.setNameFilter("Images (*.png *.jpg *.tiff)");
//    if (dialog.exec() == 1) {
//       fileString = dialog.selectedFiles()[0];
//        Image image(Image::fromFile(fileString));
//        cout<<image.getPixel(50,50);
//        int width = image.getWidth()+5;
//        int height = image.getHeight()+5;
//        ui->graphicsView->setFixedSize(width, height);
//        item ->setPixmap(QPixmap::fromImage(image.toQImage()));
//   }
//}


//void MainWindow::displayImage(Image image)
//{
//    int width = image.getWidth()+5;
//    int height = image.getHeight()+5;
//    ui->graphicsView->setFixedSize(width, height);
//    item ->setPixmap(QPixmap::fromImage(image.toQImage()));

//}

//void MainWindow::on_ReloadButton_clicked()
//{
////    image = new QImage(fileString);
////    displayImage();
//}

//void MainWindow::on_grayscaleBox_currentIndexChanged(int index)
//{

//}

//void MainWindow::on_borderWrapBox_currentIndexChanged(int index)
//{
//    switch (index) {
//    case 0:
//        borderWrapType = ZeroBorder;
//        break;
//    case 1:
//        borderWrapType = CopyBorder;
//        break;
//    case 2:
//        borderWrapType = ReflectBorder;
//        break;
//    case 3:
//        borderWrapType = WrapupBorder;
//        break;
//    default:
//        break;
//    }
//}

//void MainWindow::on_SobelXButton_clicked()
//{


//}
