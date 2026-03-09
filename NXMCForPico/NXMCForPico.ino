#include "PokeControllerForPico_Func.h"

static bool isNx2 = false;
static bool isText = false;
Proc_State_t proc_state = NONE;
USB_JoystickReport_Input_t pc_report;
int ProgState = STATE0;
static char pc_report_str[MAX_BUFFER];
static uint8_t idx = 0;
static uint32_t timeoutCnt = 0;

void setup() {
  Serial1_Init();    // RX←0でのシリアル通信
  Controller_Init(); // コントローラーの準備
  Keyboard_Init();   // キーボードの初期化
  Controller_Reset();
}

void loop() {
  tud_task(); // tinyusb device task 
  while (Serial1.available()) {
    timeoutCnt = 1;
    uint8_t c = 0;
    c = Serial1.read();

    if (c == 0xaa) {
      if (idx == 0) {
        isNx2 = true;
        isText = false;
      }
    } else if (c == 0xab) {
      if (idx == 0) {
        isNx2 = true;
        isText = false;
      }
    } else if (c == '\"') {
      if (idx == 0) {
        isText = true;
        isNx2 = false;
      }
    } else {
      if (idx == 0) {
        isNx2 = false;
        isText = false;
      }
    }

    if ((c != '\n' || isNx2 || isText) && idx < MAX_BUFFER)
      pc_report_str[idx++] = c;

    if ((c == '\r' && !isNx2 && !isText) || (isNx2 && idx == 11) || (isText && c == '\n' && pc_report_str[idx - 2] == '\r' && pc_report_str[idx - 3] == '\"')) {
      pc_report_str[idx++] = '\0';
      idx = 0;
      timeoutCnt = 0;
      ParseLine(pc_report_str);
      if (!isText && proc_state == PC_CALL) sendReportOnly(pc_report);
      memset(pc_report_str, 0, sizeof(pc_report_str));
    }
  }
  if (timeoutCnt > 1000) {
    idx = 0;
    timeoutCnt = 0;
  } else {
    if (timeoutCnt > 0) timeoutCnt++;
  }
  //状態に応じて切り替える
  //switch (ProgState)
  //{
  //  case STATE1:
  //    SwitchFunction();
  //    break;
  //  default:
  //    /* バグ回避を兼ねて状態の初期化を行う */
  //    ProgState = STATE0;
  //    break;
  //}
}

//データが利用可能な時に呼び出される関数(Serial1)
void serialEvent1()
{
  // one character comes at a time
  //if (Serial1.available()) {
  //  char c = Serial1.read();

  //  if (c == '\n')
  //  {
  //    pc_report_str[idx++] = c;
  //    pc_report_str[idx++] = '\0';
  //    ParseLine(pc_report_str);
  //    idx = 0;
  //    memset(pc_report_str, 0, sizeof(pc_report_str));
  //    ProgState = STATE1;
  //  }
  //  else if (idx < MAX_BUFFER)
  //  {
  //    pc_report_str[idx++] = c;
  //  }
  //}
}
