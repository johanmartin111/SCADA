#ifndef SISTEMA_SCADA_H
#define SISTEMA_SCADA_H

#include <QMainWindow>
#include <QProcess>
#include <QTimer>

namespace Ui {
class Sistema_scada;
}

class Sistema_scada : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sistema_scada(QWidget *parent = 0);
    ~Sistema_scada();

private slots:
    void on_boton_iniciar_clicked();
    void on_valv_entrada_sliderMoved(int position);
    void on_valv_salida_sliderMoved(int position);

    void on_Tanque();

private:
    Ui::Sistema_scada *ui;
    bool estadoboton;
    QProcess *process;
    QTimer   *thread;
    int segs;
    int valve;
    int valvs;
    int volumen;
};

#endif // SISTEMA_SCADA_H
