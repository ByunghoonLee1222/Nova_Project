using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using CsharpUSBJoystick;
using Microsoft.DirectX.DirectInput;



namespace WATSerialCom
{



    public partial class frmMain : Form
    {
        //  int vui = 0;
        SerialPort m_sp1;



        public frmMain()
        {
            InitializeComponent();
        }
        private USB_Joystick joystick;
        private string strText;
        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            // 폼 닫을 때 자원 해제하는 메서드
            joystick.Dispose();
        }

        byte[] sdData = new byte[255];
        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                // 일단 객체 생성을 시도해보고...
                // (범위는 [0, 1000], 이동 비율은 0.2퍼센트씩 => 각 방향을 500단계로 나타내겠죠)
                joystick = new USB_Joystick(0, 1000, .20, this);
            }
            catch (Exception) // 오류나면(아마도 조이스틱이 없는 경우) 예외처리
            {

                throw;
            }

            // 조이스틱 객체 사용을 위해 정해놓은 값 할당
            joystick.PresetValues();


            // 타이머 시작
            timer2.Start();
            txtComNum.Text = "COM26";
            txtBaudRate.Text = "115200";
        }

        private void btnOpen_Click(object sender, EventArgs e)
        {
            try
            {
                // m_sp1 값이 null 일때만 새로운 SerialPort 를 생성합니다.
                if (null == m_sp1)
                {
                    m_sp1 = new SerialPort();
                    m_sp1.PortName = txtComNum.Text;   // 컴포트명
                    m_sp1.BaudRate = Convert.ToInt32(txtBaudRate.Text);   // 보레이트

                    m_sp1.Open();
                }

                btnOpen.Enabled = !m_sp1.IsOpen;    // OPEN BUTTON Disable
                btnClose.Enabled = m_sp1.IsOpen;     // CLOSE BUTTON Enable
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            // m_sp1 이 null 아닐때만 close 처리를 해준다.
            if (null != m_sp1)
            {
                if (m_sp1.IsOpen)
                {
                    m_sp1.Close();
                    m_sp1.Dispose();
                    m_sp1 = null;
                }

            }
            btnOpen.Enabled = true;
            btnClose.Enabled = false;
        }

        private void btnSend_Click(object sender, EventArgs e)
        {

            // HEXA 로 보낼때 필요한 변수
            byte[] byteSendData = new byte[200];
            int iSendCount = 0;  // 헥사로 보낼때의 데이터 수
            try
            {
                if (true == checkBox1.Checked) // 헥사로 보낸다면
                {
                    foreach (string s in textBox2.Text.Split(' '))
                    {

                        if (null != s && "" != s)
                            byteSendData[iSendCount++] = Convert.ToByte(s, 16);

                    }
                    m_sp1.Write(byteSendData, 0, iSendCount);
                }
                else
                {
                    // string을 그대로 보내기
                    m_sp1.Write(textBox2.Text);
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message, "SEND 데이터 오류");
            }
        }
        /*
        try
        {
            m_sp1.Write(textBox1.Text);
            //MessageBox.Show(textBox1.Text);
                
        }
        catch (System.Exception ex)
        {
            MessageBox.Show(ex.Message);
        }
        */


        private void btnRx_Click(object sender, EventArgs e)
        {
            int iRecSize = m_sp1.BytesToRead; // 수신된 데이터 갯수
            string strRxData;
            try
            {
                if (iRecSize != 0)
                {

                    strRxData = "";
                    byte[] buff = new byte[iRecSize];

                    m_sp1.Read(buff, 0, iRecSize);
                    for (int iTemp = 0; iTemp < iRecSize; iTemp++)
                    {
                        //strRxData = String.Concat(strRxData, (buff[iTemp] - '0').ToString());
                        //strRxData = "changwoo";
                        strRxData = BitConverter.ToString(buff).Replace("-", string.Empty);
                        //byte[] decByetes1 = Encoding.UTF8.GetBytes(s1);
                    }
                    MessageBox.Show(strRxData);
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void txtComNum_TextChanged(object sender, EventArgs e)
        {

        }

  

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void foward_CheckedChanged(object sender, EventArgs e)
        {



        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void up_Click(object sender, EventArgs e)
        {
            textBox2.Text = "b7 b8 00 00 04 00 00 00 00 fa";
            this.btnSend.PerformClick();
            this.btnSend.PerformClick();
        }

        private void Demo_Click(object sender, EventArgs e)
        {

            textBox2.Text = "b7 b8 00 00 09 00 00 00 00 fa";
            this.btnSend.PerformClick();
            this.btnSend.PerformClick();
        }

        private void foward_Click(object sender, EventArgs e)
        {
            textBox2.Text = "b7 b8 00 00 05 38 ff 38 ff fa";
            this.btnSend.PerformClick();
            this.btnSend.PerformClick();
        }

        private void down_Click(object sender, EventArgs e)
        {
            textBox2.Text = "b7 b8 00 00 03 00 00 00 00 fa";
            this.btnSend.PerformClick();
            this.btnSend.PerformClick();
        }

        private void Liftstop_Click(object sender, EventArgs e)
        {
            textBox2.Text = "b7 b8 00 00 00 00 00 00 00 fa";
            this.btnSend.PerformClick();
            this.btnSend.PerformClick();
        }

        private void stop_Click(object sender, EventArgs e)
        {
            textBox2.Text = "b7 b8 00 00 05 00 00 00 00 fa";
            this.btnSend.PerformClick();
            this.btnSend.PerformClick();
        }

        private void left_Click(object sender, EventArgs e)
        {
            textBox2.Text = "b7 b8 00 00 05 c8 00 38 ff fa";
            this.btnSend.PerformClick();
            this.btnSend.PerformClick();
        }

        private void right_Click(object sender, EventArgs e)
        {
            textBox2.Text = "b7 b8 00 00 05 38 ff c8 00 fa";
            this.btnSend.PerformClick();
            this.btnSend.PerformClick();
        }

        private void backward_Click(object sender, EventArgs e)
        {
            textBox2.Text = "b7 b8 00 00 05 c8 00 c8 00 fa";
            this.btnSend.PerformClick();
            this.btnSend.PerformClick();
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            joystick.GetData();
            UpdateFormControls();  // 값 업데이트 부분을 따로 분리함

        }
        private void UpdateFormControls()
        {
            strText = null;
            // 눌린 버튼 목록을 표시할 문자열의 초기화

            // 눌린 버튼 번호를 문자열로 연결하여 한 번에 나타내주기 위해서 입니다.
            //  strText = null;

            // 축 값 : 조이스틱 객체에서 State.X, State.Y, State.Z로 축 상태를 읽어옵니다.
            X_value.Text = joystick.State.X.ToString(); // X축 값 얻어옴
            Y_value.Text = joystick.State.Y.ToString(); // Y축 값 얻어옴
            Z_value.Text = joystick.State.Z.ToString(); // Z축 값 얻어옴

            byte[] buttons = joystick.State.GetButtons();

            for (int i = 1; 1 <= 12; i++)
            {
                if (joystick.ButtonPressed(i) == true)
                {
                    strText += i + " ";
                }
                Button_value.Text = strText;

            }
        }
    }
}