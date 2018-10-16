namespace MPS
{
    partial class AddForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.SaveButton = new System.Windows.Forms.Button();
            this.BackButton = new System.Windows.Forms.Button();
            this.RelayListBox = new System.Windows.Forms.ListBox();
            this.StartTimePicker = new System.Windows.Forms.DateTimePicker();
            this.StartTimeLabel = new System.Windows.Forms.Label();
            this.EndTimeLabel = new System.Windows.Forms.Label();
            this.EndTimePicker = new System.Windows.Forms.DateTimePicker();
            this.WeekCheckedListBox = new System.Windows.Forms.CheckedListBox();
            this.WeekLabel = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // SaveButton
            // 
            this.SaveButton.Location = new System.Drawing.Point(7, 184);
            this.SaveButton.Name = "SaveButton";
            this.SaveButton.Size = new System.Drawing.Size(75, 23);
            this.SaveButton.TabIndex = 3;
            this.SaveButton.Text = "Сохранить";
            this.SaveButton.UseVisualStyleBackColor = true;
            this.SaveButton.Click += new System.EventHandler(this.SaveButton_Click);
            // 
            // BackButton
            // 
            this.BackButton.Location = new System.Drawing.Point(88, 184);
            this.BackButton.Name = "BackButton";
            this.BackButton.Size = new System.Drawing.Size(75, 23);
            this.BackButton.TabIndex = 4;
            this.BackButton.Text = "Назад";
            this.BackButton.UseVisualStyleBackColor = true;
            this.BackButton.Click += new System.EventHandler(this.BackButton_Click);
            // 
            // RelayListBox
            // 
            this.RelayListBox.FormattingEnabled = true;
            this.RelayListBox.Items.AddRange(new object[] {
            "1 устройство",
            "2 устройство",
            "3 устройство",
            "4 устройство",
            "5 устройство",
            "6 устройство",
            "7 устройство",
            "8 устройство"});
            this.RelayListBox.Location = new System.Drawing.Point(139, 70);
            this.RelayListBox.Name = "RelayListBox";
            this.RelayListBox.Size = new System.Drawing.Size(253, 108);
            this.RelayListBox.TabIndex = 5;
            // 
            // StartTimePicker
            // 
            this.StartTimePicker.Format = System.Windows.Forms.DateTimePickerFormat.Time;
            this.StartTimePicker.Location = new System.Drawing.Point(236, 18);
            this.StartTimePicker.Name = "StartTimePicker";
            this.StartTimePicker.Size = new System.Drawing.Size(156, 20);
            this.StartTimePicker.TabIndex = 2;
            // 
            // StartTimeLabel
            // 
            this.StartTimeLabel.AutoSize = true;
            this.StartTimeLabel.Location = new System.Drawing.Point(133, 18);
            this.StartTimeLabel.Name = "StartTimeLabel";
            this.StartTimeLabel.Size = new System.Drawing.Size(97, 13);
            this.StartTimeLabel.TabIndex = 7;
            this.StartTimeLabel.Text = "Начальное время";
            // 
            // EndTimeLabel
            // 
            this.EndTimeLabel.AutoSize = true;
            this.EndTimeLabel.Location = new System.Drawing.Point(136, 44);
            this.EndTimeLabel.Name = "EndTimeLabel";
            this.EndTimeLabel.Size = new System.Drawing.Size(90, 13);
            this.EndTimeLabel.TabIndex = 8;
            this.EndTimeLabel.Text = "Конечное время";
            // 
            // EndTimePicker
            // 
            this.EndTimePicker.Format = System.Windows.Forms.DateTimePickerFormat.Time;
            this.EndTimePicker.Location = new System.Drawing.Point(236, 44);
            this.EndTimePicker.Name = "EndTimePicker";
            this.EndTimePicker.Size = new System.Drawing.Size(156, 20);
            this.EndTimePicker.TabIndex = 9;
            // 
            // WeekCheckedListBox
            // 
            this.WeekCheckedListBox.CheckOnClick = true;
            this.WeekCheckedListBox.FormattingEnabled = true;
            this.WeekCheckedListBox.Items.AddRange(new object[] {
            "Понедельник",
            "Вторник",
            "Среда",
            "Четверг",
            "Пятница",
            "Суббота",
            "Воскресенье"});
            this.WeekCheckedListBox.Location = new System.Drawing.Point(7, 39);
            this.WeekCheckedListBox.Name = "WeekCheckedListBox";
            this.WeekCheckedListBox.Size = new System.Drawing.Size(114, 139);
            this.WeekCheckedListBox.TabIndex = 10;
            // 
            // WeekLabel
            // 
            this.WeekLabel.AutoSize = true;
            this.WeekLabel.Location = new System.Drawing.Point(4, 18);
            this.WeekLabel.Name = "WeekLabel";
            this.WeekLabel.Size = new System.Drawing.Size(117, 13);
            this.WeekLabel.TabIndex = 11;
            this.WeekLabel.Text = "Выберите дни недели";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(169, 189);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 12;
            this.label2.Text = "label2";
            // 
            // AddForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(397, 211);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.WeekLabel);
            this.Controls.Add(this.WeekCheckedListBox);
            this.Controls.Add(this.EndTimePicker);
            this.Controls.Add(this.EndTimeLabel);
            this.Controls.Add(this.StartTimeLabel);
            this.Controls.Add(this.RelayListBox);
            this.Controls.Add(this.BackButton);
            this.Controls.Add(this.SaveButton);
            this.Controls.Add(this.StartTimePicker);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "AddForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Добавление расписания";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.AddForm_FormClosed);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button SaveButton;
        private System.Windows.Forms.Button BackButton;
        private System.Windows.Forms.ListBox RelayListBox;
        private System.Windows.Forms.DateTimePicker StartTimePicker;
        private System.Windows.Forms.Label StartTimeLabel;
        private System.Windows.Forms.Label EndTimeLabel;
        private System.Windows.Forms.DateTimePicker EndTimePicker;
        private System.Windows.Forms.CheckedListBox WeekCheckedListBox;
        private System.Windows.Forms.Label WeekLabel;
        private System.Windows.Forms.Label label2;
    }
}