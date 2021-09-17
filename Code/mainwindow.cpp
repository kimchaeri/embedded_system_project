#include "mainwindow.h"
#include "ui_mainwindow.h"

pthread_mutex_t mutex;

void *t_function1(void *data); //temp control-egg
void *t_function2(void *data); //temp check-egg
void *t_functuon3(void *data); //sun-adolescent,old
void *t_function4(void *data); //flash,temp,oxygen control-adolescent,old
void *t_function5(void *data); //temp oxygen check-adolescent,old
void *t_function6(void *data); //current-old
void text_print(void); //temp, oxygen print

int temp=7;
int oxygen=120;
int stress=-1;
int led_data[8]={1,3,7,15,31,63,127,256};
int num=0;

int temp_th1 = 2; //thread1번을 정지시켜주기 위한 임시변수
int temp_th2 = 2;
int thr_id;
int status;
void *tret_temp = NULL;
void *tret = NULL;
void *tret_stress = NULL;
void *tret_sun = NULL;
void *tret_current = NULL;
void *tret_check = NULL;

pthread_t p_thread[5];
char p1[] = "thread_1"; //temp control-egg
char p2[] = "thread_2"; //temp check-egg
char p3[] = "thread_3"; //sun-adolescent,old
char p4[] = "thread_4"; //flash,temp,oxygen control-adolescent,old
char p5[] = "thread_5"; //temp oxygen check-adolescent,old
char p6[] = "thread_6"; //current-old

int text_lcd_dev;
int push_switch_dev;
int led_dev;
int dot_dev;
int step_motor_dev;
int *result;

unsigned char push_sw_buff[MAX_BUTTON];
int buff_size = sizeof(push_sw_buff);
unsigned char sun_dot[3][10] = {
    {0x00,0x08,0x22,0x1d,0x36,0x36,0x1d,0x22,0x08,0x00},
    {0x08,0x22,0x1d,0x3e,0x14,0x14,0x3e,0x1d,0x22,0x08},
    {0x2a,0x5d,0x3e,0x1d,0x77,0x77,0x1d,0x3e,0x5d,0x2a}
};
unsigned char setzero[10] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char motor_state[3];
int run(int n);
void clear_led();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setFixedSize(350, 600);
    ui->setupUi(this);

    QMessageBox msgBox;
    text_lcd_dev = open(FPGA_TEXT_LCD_DEVICE,O_RDWR);
    push_switch_dev = open(FPGA_PUSH_SWITCH_DEVICE,O_RDWR);
    led_dev = open(LED_DEVICE,O_RDWR);
    step_motor_dev = open(FPGA_STEP_MOTOR_DEVICE,O_RDWR);\
    dot_dev = open(FPGA_DOT_DEVICE, O_WRONLY);

    if(text_lcd_dev<0){
        printf("Device open error:%s\n",FPGA_TEXT_LCD_DEVICE);
        msgBox.setText("Device Open Error!");
        msgBox.setInformativeText("Check Module & DEVICE node");
        msgBox.exec();
        exit(1);
    }
    if(push_switch_dev<0){
        printf("Device open error:%s\n",FPGA_PUSH_SWITCH_DEVICE);
        msgBox.setText("Device Open Error!");
        msgBox.setInformativeText("Check Module & DEVICE node");
        msgBox.exec();
        exit(1);
    }
    if(led_dev<0){
        printf("Device open error:%s\n",LED_DEVICE);
        msgBox.setText("Device Open Error!");
        msgBox.setInformativeText("Check Module & DEVICE node");
        msgBox.exec();
        exit(1);
    }
    if(step_motor_dev<0){
        printf("Device open error:%s\n",FPGA_STEP_MOTOR_DEVICE);
        msgBox.setText("Device Open Error!");
        msgBox.setInformativeText("Check Module & DEVICE node");
        msgBox.exec();
        exit(1);
    }


    int w = ui->label_pic->width();
    int h = ui->label_pic->height();

    QPixmap pix1("/home/pi/egg/bokcci_start.jpg");
    ui->label_pic->setPixmap(pix1);
    ui->label_pic->setPixmap(pix1.scaled(w,h));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int w = ui->label_pic->width();
    int h = ui->label_pic->height();

    QPixmap pix2("/home/pi/egg/egg.png");
    ui->label_pic->setPixmap(pix2);
    ui->label_pic->setPixmap(pix2.scaled(w,h));
    QApplication::processEvents();

    pthread_mutex_init(&mutex, NULL);

    clear_led();
    int retVal1=run(1);
    if(retVal1==0){//grow up
        QPixmap pix3("/home/pi/egg/adolescent_bokcci.png");
        ui->label_pic->setPixmap(pix3);
        ui->label_pic->setPixmap(pix3.scaled(w,h));
    }
    else if(retVal1==1){//die because it cannot hatch from the egg
        QPixmap pix4("/home/pi/egg/die_cannothatch.jpg");
        ui->label_pic->setPixmap(pix4);
        ui->label_pic->setPixmap(pix4.scaled(w,h));
        return;
    }
    clear_led();
    QApplication::processEvents();
    num = 0;
    int retVal2=run(2);
    temp_th2=2;
    if(retVal2 == 1) {
         QPixmap pix5("/home/pi/egg/die_sun.png");
         ui->label_pic->setPixmap(pix5);
         ui->label_pic->setPixmap(pix5.scaled(w,h));
         return;
     }
     else if(retVal2 == 5) {
         QPixmap pix6("/home/pi/egg/die_stress.jpg");
         ui->label_pic->setPixmap(pix6);
         ui->label_pic->setPixmap(pix6.scaled(w,h));
         return;
     }
     else if(retVal2 == 2){
        QPixmap pix7("/home/pi/egg/old_bokcci.png");
        ui->label_pic->setPixmap(pix7);
        ui->label_pic->setPixmap(pix7.scaled(w,h));
     }
    QApplication::processEvents();
    int retVal3=run(3);
    if(retVal3 == 1) {
        QPixmap pix8("/home/pi/egg/die_sun.png");
        ui->label_pic->setPixmap(pix8);
        ui->label_pic->setPixmap(pix8.scaled(w,h));
        return;
    }
    else if(retVal3 == 3) {
        QPixmap pix9("/home/pi/egg/water.jpg");
        ui->label_pic->setPixmap(pix9);
        ui->label_pic->setPixmap(pix9.scaled(w,h));
        return;
    }
    else if(retVal3 == 4) {
        QPixmap pix10("/home/pi/egg/die.jpg");
        ui->label_pic->setPixmap(pix10);
        ui->label_pic->setPixmap(pix10.scaled(w,h));
        return;
    }
    else if(retVal3 == 5) {
        QPixmap pix11("/home/pi/egg/die_stress.jpg");
        ui->label_pic->setPixmap(pix11);
        ui->label_pic->setPixmap(pix11.scaled(w,h));
        return;
    }
    else if(retVal3 == 6) {
        QPixmap pix12("/home/pi/egg/flash.jpg");
        ui->label_pic->setPixmap(pix12);
        ui->label_pic->setPixmap(pix12.scaled(w,h));
        return;
    }
    pthread_mutex_destroy(&mutex);
}

void *t_function1(void *data) //temp control-egg
{
    text_print();
    printf("read button\n");
    while(1){
        read(push_switch_dev, &push_sw_buff, buff_size);
        if(temp_th1 == 0|| temp_th1 == 1) {
            printf("exit\n");
            pthread_exit(0);
        }
        //push_sw_buff[6]==1
        if(push_sw_buff[6]==1){
            pthread_mutex_lock(&mutex);
            temp = temp+1;
            text_print();
            pthread_mutex_unlock(&mutex);
        }
        //push_sw_buff[7]==1
        if(push_sw_buff[7]==1){
            pthread_mutex_lock(&mutex);
            temp = temp-1;
            text_print();
            pthread_mutex_unlock(&mutex);
        }
        sleep(1);
    }

}

void *t_function2(void *data){ //temp check-egg
    static int retval=0;
    int count = 1;
    while(count <= (*((int *)data))*10){
        sleep(3);
        if(temp>20 || temp<10){
            write(led_dev,&led_data[num],1);
            num++;
        }
        if(num==8){
            retval=1;
            temp_th1 = 1;
            pthread_exit((void*)&retval);
        }
        count++;
    }
    temp_th1 = 0;
    pthread_exit((void*)&retval);
}

void *t_function3(void *data){ //sun-adolescent,old
    srand(time(NULL));
        int sun_value = 0;
        static int retval = 999;
        int count = 1;
        write(dot_dev, setzero, sizeof(setzero));
        while(count <= (*((int *)data))*2) {
            sleep(30);
            if(sun_value==3) {
                pthread_exit((void*)&retval);
            }
            sun_value=rand()%3+1;
            retval = sun_value;
            write(dot_dev, sun_dot[sun_value-1], sizeof(sun_dot[sun_value-1]));
            sleep(1);
            count ++;
        }
        pthread_exit((void*)&retval);
}

void *t_function4(void *data) //flash,temp,oxygen control-adolescent,old
{
    char *string = (char *)data;
    int a = 0;
    int retval = 0;

    while(1){
        if(temp_th2 == 0|| temp_th2 == 1) {
                    printf("exit\n");
                    pthread_exit((void*)&retval);  //stress x temp_th2 = 1, retv = 0
        }

        read(push_switch_dev, &push_sw_buff, buff_size);

        //push_sw_buff[0]==1
        if(push_sw_buff[0]==1){
            retval = 0;
            temp_th2 = -1;
            pthread_exit((void*)&retval);  //flash temp_th2 = -1, retv = 0
        }
        //push_sw_buff[1]==1
        if(push_sw_buff[1]==1){
            stress = stress+1;
            /*LED function*/
            write(led_dev,&led_data[stress], 1);
            if(stress > 6){
                retval = 2;
                temp_th2 = 0;
                pthread_exit((void*)&retval);  //stress o temp_th2 = 0, retv = 2
            }
        }
        //push_sw_buff[3]==1
        if(push_sw_buff[3]==1){
           oxygen = oxygen+10;
           text_print();
        }
        //push_sw_buff[4]==1
        if(push_sw_buff[4]==1){
           oxygen = oxygen-10;
           text_print();
        }
        //push_sw_buff[6]==1
        if(push_sw_buff[6]==1){
            temp = temp+1;
            text_print();
        }
        //push_sw_buff[7]==1
        if(push_sw_buff[7]==1){
            temp = temp-1;
            text_print();
        }
        sleep(1);
    }
}

void *t_function5(void *data) { //temp oxygen check-adolescent,old
    int retv = 999;
    srand(time(NULL));
    int count = 1;
    while(count <= (*((int *)data))*3) {
        if(temp_th2 == 0|| temp_th2 == 1 || temp_th2 == -1) {
            retv = 0;
            printf("exit\n");
            pthread_exit((void*)&retv); //stress x temp_th2 = 1, retv = 0
        }
        if(num == 8) { //stress == 8
            retv = 2;
            temp_th2 = 0;
            pthread_exit((void*)&retv);  //stress o temp_th2 = 0, retv = 2
        }
        sleep(20);
        pthread_mutex_lock(&mutex);
        temp = rand()%40+10;
        oxygen = rand()%150 + 20;
        text_print();
        pthread_mutex_unlock(&mutex);
        if(temp < 20 || temp > 30 || oxygen < 70 || oxygen > 150) {
            write(led_dev,&led_data[num],1);
            num++;
        }
        count ++;
    }
    retv = 0;
    temp_th2 = 1;
    pthread_exit((void*)&retv); //stress x temp_th2 = 1, retv = 0
}

void *t_function6(void *data) //current-old
{
    srand(time(NULL));
    int water_value = 0;
    static int retv = 0;
    int temp =1;
    int temp_not = 0;
    memset(motor_state,0,sizeof(motor_state));
    motor_state[0] = (unsigned  char)temp_not;
    motor_state[1] = (unsigned  char)temp_not;
    motor_state[2] = (unsigned  char)temp_not;
    write(step_motor_dev, motor_state, 3);

    int count = 1;
    while(count <= (*((int *)data))*2) {
        sleep(30);
        if(water_value > 150) {
            pthread_exit((void*)&retv);
        }
        water_value=rand()%(170+1-100)+100;
        retv = water_value;
        printf("water_value = %d\n", water_value);
        memset(motor_state,0,sizeof(motor_state));
        motor_state[0] = (unsigned  char)temp;
        motor_state[1] = (unsigned  char)temp;
        motor_state[2] = (unsigned  char)water_value;
        write(step_motor_dev, motor_state, 3);
        count ++;
    }
    pthread_exit((void*)&retv);
}

int run(int t) {
    if(t == 1) {
        thr_id = pthread_create(&p_thread[1], NULL, t_function2, (void *)&t);
        thr_id = pthread_create(&p_thread[0], NULL, t_function1, (void *)&t);
        if(thr_id<0){
            perror("thread create error : ");
            exit(0);
        }
        pthread_join(p_thread[1], &tret);
        printf("return value : %d\n",*((int*)tret) );
        if(temp_th1 == 0|| temp_th1 == 1) pthread_cancel(p_thread[0]);
        if(*((int*)tret) == 0) return 0;
        else  return 1;
    }

    if(t == 2) {
        //thr_id = pthread_create(&p_thread[2], NULL, t_function3, (void *)&t);
        thr_id = pthread_create(&p_thread[3], NULL, t_function4, (void *)p5);
        thr_id = pthread_create(&p_thread[4], NULL, t_function5, (void *)&t);
        if(thr_id<0) {
            perror("thread create error : ");
            exit(0);
        }
        //pthread_join(p_thread[2], &tret_sun);
        pthread_join(p_thread[4], &tret_stress);
        if(temp_th2 == -1) return 6;
        if(*((int*)tret_stress) == 2 || temp_th2 == 0 ) return 5;
        //if(*((int*)tret_sun) == 3) return 1;
        return 2;
    }
    else if(t == 3) {
           //thr_id = pthread_create(&p_thread[2], NULL, t_function3, (void *)&t);
           //thr_id = pthread_create(&p_thread[5], NULL, t_function6, (void *)&t);
           thr_id = pthread_create(&p_thread[3], NULL, t_function4, (void *)p5);
           thr_id = pthread_create(&p_thread[4], NULL, t_function5, (void *)&t);
           if(thr_id<0) {
               perror("thread create error : ");
               exit(0);
           }
           //pthread_join(p_thread[2], &tret_sun);
           //pthread_join(p_thread[5], &tret_current);
           pthread_join(p_thread[3], &tret_stress);
           if(temp_th2 == -1) return 6;
           if(*((int*)tret_stress) == 2 || temp_th2 == 0 ) return 5;
           //if((*((int*)tret_sun)) == 3) return 1; //die because of sun
           //if((*((int*)tret_current)) > 150) return 3; //die because of current
           return 4; //die_success
    }

    return 0;
}

void clear_led() {
    for(int i = 0;i<8;i++) {
        write(led_dev,&led_data[i],0);
    }
}

void text_print(void){
    int temp_size;
    int oxygen_size;
    char string[32];
    char temp_string[16]="TEMP:";
    char oxygen_string[16]="OXYGEN:";
    char str1[20];//temp value
    char str2[20];//oxygen value

    sprintf(str1, "%d", temp);
    strncat(temp_string, str1, strlen(str1));
    temp_size = strlen(temp_string);
    strncat(string, temp_string, temp_size);
    memset(string+temp_size, ' ', LINE_BUFF-temp_size);

    sprintf(str2, "%d", oxygen);
    strncat(oxygen_string, str2, strlen(str2));
    oxygen_size = strlen(oxygen_string);
    strncat(string, oxygen_string, oxygen_size);
    memset(string+LINE_BUFF+oxygen_size, ' ', LINE_BUFF-oxygen_size);

    write(text_lcd_dev, string, MAX_BUFF);
}

void MainWindow::on_Close_BT_clicked()
{
    this->close();
}
