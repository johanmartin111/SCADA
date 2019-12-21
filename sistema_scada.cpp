#include "sistema_scada.h"
#include "ui_sistema_scada.h"

#include <QProcess>
#include <QObject>
#include <QDebug>


//  CONSTRUCTOR
Sistema_scada::Sistema_scada(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sistema_scada)
{
    ui->setupUi(this);
    estadoboton = false;
    ui->ledVerde->setEnabled(false);
    //ui->ledRojo->setEnabled(false);

    // PROCESO
    process = new QProcess(this);
    thread  = new QTimer(this);
    valve   = ui->valv_entrada->value();
    valvs   = ui->valv_salida->value();
    volumen = ui->Tanque->value();
    segs    = 0;
}
//  CONSTRUCTOR


//  DESTRUCTOR
Sistema_scada::~Sistema_scada()
{
    delete ui;
}
//  DESTRUCTOR


//  CLICK BOTON
void Sistema_scada::on_boton_iniciar_clicked()
{
    if ( !estadoboton ){
      ui->boton_iniciar->setText("Pausar");
      ui->boton_iniciar->setIcon(QIcon("://imagenes/pausa.png"));
      estadoboton = true;

      // EJECUTA PROCESO PRINCIPAL
      QString valve = QString::number(ui->valv_entrada->value()); // valvula entrada
      QString valvs = QString::number(ui->valv_salida->value());  // valvula salida
      QStringList args;
      args << "-valve" << valve << "-vals" << valvs;

      process->start("./Tanque/panel", args);  // inicia proceso con valores iniciales de las valvulas
      process ->waitForStarted();              // espera que se inicie

      connect(thread, SIGNAL(timeout()), this, SLOT(on_Tanque()));
      thread->start(100);  //  10 VECES POR SEGUNDO
      //EJECUTA PROCESO PRINCIPAL

    } else {
      //  DETIENE PROCESO
      process->close();
      thread->stop();

      ui->boton_iniciar->setText("Iniciar");
      ui->boton_iniciar->setIcon(QIcon("://imagenes/play.png"));
      estadoboton = false;
    }
}
//  CLICK BOTON


// VALV SLIDER
void Sistema_scada::on_valv_entrada_sliderMoved(int position)
{
    valve = position;
    ui->l_valve->setText(QString::number(position) + " lts/s");
}

void Sistema_scada::on_valv_salida_sliderMoved(int position)
{
    valvs = position;
    ui->l_valvs->setText(QString::number(position)+ " lts/s");
}
// VALV SLIDER


//  TANQUE
void Sistema_scada::on_Tanque()
{
    volumen = volumen + (valve/10) - (valvs/10);
    qDebug() << QString::number(segs++) + "s"
             << "Entrada: " + QString::number(valve) + " lts/s"
             << "Salida: "  + QString::number(valvs) + " lts/s"
             << "Volumen: " + QString::number(volumen)  + " lts";

    if (volumen >= 1000){  //  SI LLEGA A MAX TOPE
        volumen = 1000;

        valve = 0;
        ui->valv_entrada->setValue(0);
        ui->l_valve->setText("0 lts/s");
        ui->ledVerde->setEnabled(true);
    } else if (volumen <= 100){  //  SI LLEGA A MIN TOPE
        volumen = 100;

        valvs = 0;
        ui->valv_salida->setValue(0);
        ui->l_valvs->setText("0 lts/s");
        ui->ledRojo->setEnabled(true);
    } else {
        ui->ledVerde->setEnabled(false);
        ui->ledRojo->setEnabled(false);
    }

    ui->volumen->display(volumen);
    ui->Tanque->setValue(volumen);
}
//TANQUE
