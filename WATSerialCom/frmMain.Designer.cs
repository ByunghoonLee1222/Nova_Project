namespace WATSerialCom
{
    partial class frmMain
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.txtComNum = new System.Windows.Forms.TextBox();
            this.txtBaudRate = new System.Windows.Forms.TextBox();
            this.btnOpen = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.Button();
            this.btnSend = new System.Windows.Forms.Button();
            this.btnRx = new System.Windows.Forms.Button();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.backward = new System.Windows.Forms.Button();
            this.right = new System.Windows.Forms.Button();
            this.stop = new System.Windows.Forms.Button();
            this.left = new System.Windows.Forms.Button();
            this.foward = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.Liftstop = new System.Windows.Forms.Button();
            this.Second = new System.Windows.Forms.Button();
            this.down = new System.Windows.Forms.Button();
            this.up = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.Button_value = new System.Windows.Forms.TextBox();
            this.Z_value = new System.Windows.Forms.TextBox();
            this.Y_value = new System.Windows.Forms.TextBox();
            this.X_value = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(19, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "Port:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(19, 47);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 12);
            this.label2.TabIndex = 0;
            this.label2.Text = "Baud:";
            // 
            // txtComNum
            // 
            this.txtComNum.Location = new System.Drawing.Point(68, 20);
            this.txtComNum.Name = "txtComNum";
            this.txtComNum.Size = new System.Drawing.Size(81, 21);
            this.txtComNum.TabIndex = 1;
            this.txtComNum.TextChanged += new System.EventHandler(this.txtComNum_TextChanged);
            // 
            // txtBaudRate
            // 
            this.txtBaudRate.Location = new System.Drawing.Point(68, 44);
            this.txtBaudRate.Name = "txtBaudRate";
            this.txtBaudRate.Size = new System.Drawing.Size(81, 21);
            this.txtBaudRate.TabIndex = 1;
            // 
            // btnOpen
            // 
            this.btnOpen.Location = new System.Drawing.Point(173, 20);
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.Size = new System.Drawing.Size(90, 34);
            this.btnOpen.TabIndex = 2;
            this.btnOpen.Text = "OPEN";
            this.btnOpen.UseVisualStyleBackColor = true;
            this.btnOpen.Click += new System.EventHandler(this.btnOpen_Click);
            // 
            // btnClose
            // 
            this.btnClose.Location = new System.Drawing.Point(173, 60);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(90, 34);
            this.btnClose.TabIndex = 2;
            this.btnClose.Text = "CLOSE";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // btnSend
            // 
            this.btnSend.Location = new System.Drawing.Point(6, 160);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(90, 34);
            this.btnSend.TabIndex = 2;
            this.btnSend.Text = "SEND";
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.btnSend_Click);
            // 
            // btnRx
            // 
            this.btnRx.Location = new System.Drawing.Point(173, 160);
            this.btnRx.Name = "btnRx";
            this.btnRx.Size = new System.Drawing.Size(90, 34);
            this.btnRx.TabIndex = 2;
            this.btnRx.Text = "RX";
            this.btnRx.UseVisualStyleBackColor = true;
            this.btnRx.Click += new System.EventHandler(this.btnRx_Click);
            // 
            // serialPort1
            // 
            this.serialPort1.BaudRate = 115200;
            this.serialPort1.PortName = "COM4";
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(6, 111);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(53, 16);
            this.checkBox1.TabIndex = 4;
            this.checkBox1.Text = "Hexa";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(6, 133);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(257, 21);
            this.textBox2.TabIndex = 5;
            this.textBox2.TextChanged += new System.EventHandler(this.textBox2_TextChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBox2);
            this.groupBox1.Controls.Add(this.checkBox1);
            this.groupBox1.Controls.Add(this.btnRx);
            this.groupBox1.Controls.Add(this.btnSend);
            this.groupBox1.Controls.Add(this.btnClose);
            this.groupBox1.Controls.Add(this.btnOpen);
            this.groupBox1.Controls.Add(this.txtBaudRate);
            this.groupBox1.Controls.Add(this.txtComNum);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(22, 27);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(280, 218);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "groupBox1";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.backward);
            this.groupBox2.Controls.Add(this.right);
            this.groupBox2.Controls.Add(this.stop);
            this.groupBox2.Controls.Add(this.left);
            this.groupBox2.Controls.Add(this.foward);
            this.groupBox2.Location = new System.Drawing.Point(330, 36);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(291, 208);
            this.groupBox2.TabIndex = 15;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "MoveControl";
            // 
            // backward
            // 
            this.backward.Location = new System.Drawing.Point(113, 148);
            this.backward.Name = "backward";
            this.backward.Size = new System.Drawing.Size(70, 41);
            this.backward.TabIndex = 0;
            this.backward.Text = "backward";
            this.backward.UseVisualStyleBackColor = true;
            this.backward.Click += new System.EventHandler(this.backward_Click);
            // 
            // right
            // 
            this.right.Location = new System.Drawing.Point(212, 89);
            this.right.Name = "right";
            this.right.Size = new System.Drawing.Size(55, 41);
            this.right.TabIndex = 0;
            this.right.Text = "right";
            this.right.UseVisualStyleBackColor = true;
            this.right.Click += new System.EventHandler(this.right_Click);
            // 
            // stop
            // 
            this.stop.Location = new System.Drawing.Point(113, 89);
            this.stop.Name = "stop";
            this.stop.Size = new System.Drawing.Size(70, 41);
            this.stop.TabIndex = 0;
            this.stop.Text = "stop";
            this.stop.UseVisualStyleBackColor = true;
            this.stop.Click += new System.EventHandler(this.stop_Click);
            // 
            // left
            // 
            this.left.Location = new System.Drawing.Point(31, 89);
            this.left.Name = "left";
            this.left.Size = new System.Drawing.Size(55, 41);
            this.left.TabIndex = 0;
            this.left.Text = "left";
            this.left.UseVisualStyleBackColor = true;
            this.left.Click += new System.EventHandler(this.left_Click);
            // 
            // foward
            // 
            this.foward.Location = new System.Drawing.Point(113, 35);
            this.foward.Name = "foward";
            this.foward.Size = new System.Drawing.Size(70, 41);
            this.foward.TabIndex = 0;
            this.foward.Text = "foward";
            this.foward.UseVisualStyleBackColor = true;
            this.foward.Click += new System.EventHandler(this.foward_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.Liftstop);
            this.groupBox3.Controls.Add(this.Second);
            this.groupBox3.Controls.Add(this.down);
            this.groupBox3.Controls.Add(this.up);
            this.groupBox3.Location = new System.Drawing.Point(639, 36);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(190, 208);
            this.groupBox3.TabIndex = 16;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "LiftControl";
            // 
            // Liftstop
            // 
            this.Liftstop.Location = new System.Drawing.Point(27, 92);
            this.Liftstop.Name = "Liftstop";
            this.Liftstop.Size = new System.Drawing.Size(55, 38);
            this.Liftstop.TabIndex = 1;
            this.Liftstop.Text = "Liftstop";
            this.Liftstop.UseVisualStyleBackColor = true;
            this.Liftstop.Click += new System.EventHandler(this.Liftstop_Click);
            // 
            // Second
            // 
            this.Second.Location = new System.Drawing.Point(97, 89);
            this.Second.Name = "Second";
            this.Second.Size = new System.Drawing.Size(78, 41);
            this.Second.TabIndex = 0;
            this.Second.Text = "Second";
            this.Second.UseVisualStyleBackColor = true;
            this.Second.Click += new System.EventHandler(this.Demo_Click);
            // 
            // down
            // 
            this.down.Location = new System.Drawing.Point(27, 148);
            this.down.Name = "down";
            this.down.Size = new System.Drawing.Size(55, 41);
            this.down.TabIndex = 0;
            this.down.Text = "down";
            this.down.UseVisualStyleBackColor = true;
            this.down.Click += new System.EventHandler(this.down_Click);
            // 
            // up
            // 
            this.up.Location = new System.Drawing.Point(27, 35);
            this.up.Name = "up";
            this.up.Size = new System.Drawing.Size(55, 41);
            this.up.TabIndex = 0;
            this.up.Text = "up";
            this.up.UseVisualStyleBackColor = true;
            this.up.Click += new System.EventHandler(this.up_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.Button_value);
            this.groupBox4.Controls.Add(this.Z_value);
            this.groupBox4.Controls.Add(this.Y_value);
            this.groupBox4.Controls.Add(this.X_value);
            this.groupBox4.Controls.Add(this.label5);
            this.groupBox4.Controls.Add(this.label4);
            this.groupBox4.Controls.Add(this.label3);
            this.groupBox4.Location = new System.Drawing.Point(22, 270);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(269, 208);
            this.groupBox4.TabIndex = 17;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Joystick";
            // 
            // Button_value
            // 
            this.Button_value.Location = new System.Drawing.Point(24, 168);
            this.Button_value.Name = "Button_value";
            this.Button_value.Size = new System.Drawing.Size(218, 21);
            this.Button_value.TabIndex = 2;
            // 
            // Z_value
            // 
            this.Z_value.Location = new System.Drawing.Point(89, 111);
            this.Z_value.Name = "Z_value";
            this.Z_value.Size = new System.Drawing.Size(60, 21);
            this.Z_value.TabIndex = 1;
            // 
            // Y_value
            // 
            this.Y_value.Location = new System.Drawing.Point(89, 77);
            this.Y_value.Name = "Y_value";
            this.Y_value.Size = new System.Drawing.Size(60, 21);
            this.Y_value.TabIndex = 1;
            // 
            // X_value
            // 
            this.X_value.Location = new System.Drawing.Point(89, 44);
            this.X_value.Name = "X_value";
            this.X_value.Size = new System.Drawing.Size(60, 21);
            this.X_value.TabIndex = 1;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(32, 114);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(12, 12);
            this.label5.TabIndex = 0;
            this.label5.Text = "z";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(32, 80);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(12, 12);
            this.label4.TabIndex = 0;
            this.label4.Text = "y";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(32, 47);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(12, 12);
            this.label3.TabIndex = 0;
            this.label3.Text = "x";
            // 
            // timer2
            // 
            this.timer2.Enabled = true;
            this.timer2.Tick += new System.EventHandler(this.timer2_Tick);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1004, 539);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "frmMain";
            this.Text = "JoyStick_Test.ver";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtComNum;
        private System.Windows.Forms.TextBox txtBaudRate;
        private System.Windows.Forms.Button btnOpen;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.Button btnSend;
        private System.Windows.Forms.Button btnRx;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button backward;
        private System.Windows.Forms.Button right;
        private System.Windows.Forms.Button stop;
        private System.Windows.Forms.Button left;
        private System.Windows.Forms.Button foward;
        private System.Windows.Forms.Button down;
        private System.Windows.Forms.Button up;
        private System.Windows.Forms.Button Second;
        private System.Windows.Forms.Button Liftstop;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.TextBox Button_value;
        private System.Windows.Forms.TextBox Z_value;
        private System.Windows.Forms.TextBox Y_value;
        private System.Windows.Forms.TextBox X_value;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Timer timer2;
    }
}

