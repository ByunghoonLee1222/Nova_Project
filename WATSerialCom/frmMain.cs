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
            // �� ���� �� �ڿ� �����ϴ� �޼���
            joystick.Dispose();
        }

        byte[] sdData = new byte[255];
        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                // �ϴ� ��ü ������ �õ��غ���...
                // (������ [0, 1000], �̵� ������ 0.2�ۼ�Ʈ�� => �� ������ 500�ܰ�� ��Ÿ������)
                joystick = new USB_Joystick(0, 1000, .20, this);
            }
            catch (Exception) // ��������(�Ƹ��� ���̽�ƽ�� ���� ���) ����ó��
            {

                throw;
            }

            // ���̽�ƽ ��ü ����� ���� ���س��� �� �Ҵ�
            joystick.PresetValues();


            // Ÿ�̸� ����
            timer2.Start();
            txtComNum.Text = "COM26";
            txtBaudRate.Text = "115200";
        }

        private void btnOpen_Click(object sender, EventArgs e)
        {
            try
            {
                // m_sp1 ���� null �϶��� ���ο� SerialPort �� �����մϴ�.
                if (null == m_sp1)
                {
                    m_sp1 = new SerialPort();
                    m_sp1.PortName = txtComNum.Text;   // ����Ʈ��
                    m_sp1.BaudRate = Convert.ToInt32(txtBaudRate.Text);   // ������Ʈ

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
            // m_sp1 �� null �ƴҶ��� close ó���� ���ش�.
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

            // HEXA �� ������ �ʿ��� ����
            byte[] byteSendData = new byte[200];
            int iSendCount = 0;  // ���� �������� ������ ��
            try
            {
                if (true == checkBox1.Checked) // ���� �����ٸ�
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
                    // string�� �״�� ������
                    m_sp1.Write(textBox2.Text);
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message, "SEND ������ ����");
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
            int iRecSize = m_sp1.BytesToRead; // ���ŵ� ������ ����
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
            UpdateFormControls();  // �� ������Ʈ �κ��� ���� �и���

        }
        private void UpdateFormControls()
        {
            strText = null;
            // ���� ��ư ����� ǥ���� ���ڿ��� �ʱ�ȭ

            // ���� ��ư ��ȣ�� ���ڿ��� �����Ͽ� �� ���� ��Ÿ���ֱ� ���ؼ� �Դϴ�.
            //  strText = null;

            // �� �� : ���̽�ƽ ��ü���� State.X, State.Y, State.Z�� �� ���¸� �о�ɴϴ�.
            X_value.Text = joystick.State.X.ToString(); // X�� �� ����
            Y_value.Text = joystick.State.Y.ToString(); // Y�� �� ����
            Z_value.Text = joystick.State.Z.ToString(); // Z�� �� ����

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