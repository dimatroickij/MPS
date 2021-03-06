﻿namespace MPS
{
    partial class WorkForm
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
            this.dataGridView = new System.Windows.Forms.DataGridView();
            this.startTime = new MPS.TimeColumn();
            this.EndTime = new MPS.TimeColumn();
            this.NumDevice = new System.Windows.Forms.DataGridViewComboBoxColumn();
            this.weekMonday = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.weekTuesday = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.weekWednesday = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.weekThursday = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.weekFriday = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.weekSaturday = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.weekSunday = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.editRecord = new System.Windows.Forms.DataGridViewButtonColumn();
            this.deleteRecord = new System.Windows.Forms.DataGridViewButtonColumn();
            this.timeColumn1 = new MPS.TimeColumn();
            this.timeColumn2 = new MPS.TimeColumn();
            this.dataGridViewTextBoxColumn1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.AddButton = new System.Windows.Forms.Button();
            this.deleteButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGridView
            // 
            this.dataGridView.AllowUserToAddRows = false;
            this.dataGridView.AllowUserToDeleteRows = false;
            this.dataGridView.AllowUserToResizeColumns = false;
            this.dataGridView.AllowUserToResizeRows = false;
            this.dataGridView.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dataGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.startTime,
            this.EndTime,
            this.NumDevice,
            this.weekMonday,
            this.weekTuesday,
            this.weekWednesday,
            this.weekThursday,
            this.weekFriday,
            this.weekSaturday,
            this.weekSunday,
            this.editRecord,
            this.deleteRecord});
            this.dataGridView.EditMode = System.Windows.Forms.DataGridViewEditMode.EditOnEnter;
            this.dataGridView.Location = new System.Drawing.Point(12, 12);
            this.dataGridView.Name = "dataGridView";
            this.dataGridView.Size = new System.Drawing.Size(1244, 375);
            this.dataGridView.TabIndex = 0;
            this.dataGridView.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView_CellContentClick);
            // 
            // startTime
            // 
            this.startTime.HeaderText = "Начальное время";
            this.startTime.Name = "startTime";
            this.startTime.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // EndTime
            // 
            this.EndTime.HeaderText = "Конечное время";
            this.EndTime.Name = "EndTime";
            this.EndTime.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // NumDevice
            // 
            this.NumDevice.AutoComplete = false;
            this.NumDevice.HeaderText = "Номер реле";
            this.NumDevice.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8"});
            this.NumDevice.Name = "NumDevice";
            this.NumDevice.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // weekMonday
            // 
            this.weekMonday.HeaderText = "Понедельник";
            this.weekMonday.Name = "weekMonday";
            this.weekMonday.TrueValue = "";
            // 
            // weekTuesday
            // 
            this.weekTuesday.HeaderText = "Вторник";
            this.weekTuesday.Name = "weekTuesday";
            this.weekTuesday.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // weekWednesday
            // 
            this.weekWednesday.HeaderText = "Среда";
            this.weekWednesday.Name = "weekWednesday";
            // 
            // weekThursday
            // 
            this.weekThursday.HeaderText = "Четверг";
            this.weekThursday.Name = "weekThursday";
            // 
            // weekFriday
            // 
            this.weekFriday.HeaderText = "Пятница";
            this.weekFriday.Name = "weekFriday";
            // 
            // weekSaturday
            // 
            this.weekSaturday.HeaderText = "Суббота";
            this.weekSaturday.Name = "weekSaturday";
            this.weekSaturday.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // weekSunday
            // 
            this.weekSunday.HeaderText = "Воскресенье";
            this.weekSunday.Name = "weekSunday";
            // 
            // editRecord
            // 
            this.editRecord.HeaderText = "";
            this.editRecord.Name = "editRecord";
            this.editRecord.Text = "Изменить";
            // 
            // deleteRecord
            // 
            this.deleteRecord.HeaderText = "";
            this.deleteRecord.Name = "deleteRecord";
            this.deleteRecord.Text = "Удалить";
            // 
            // timeColumn1
            // 
            this.timeColumn1.HeaderText = "Начальное время";
            this.timeColumn1.Name = "timeColumn1";
            this.timeColumn1.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // timeColumn2
            // 
            this.timeColumn2.HeaderText = "Конечное время";
            this.timeColumn2.Name = "timeColumn2";
            this.timeColumn2.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // dataGridViewTextBoxColumn1
            // 
            this.dataGridViewTextBoxColumn1.HeaderText = "Конечное время";
            this.dataGridViewTextBoxColumn1.Name = "dataGridViewTextBoxColumn1";
            // 
            // dataGridViewTextBoxColumn2
            // 
            this.dataGridViewTextBoxColumn2.HeaderText = "Номер реле";
            this.dataGridViewTextBoxColumn2.Name = "dataGridViewTextBoxColumn2";
            // 
            // AddButton
            // 
            this.AddButton.Location = new System.Drawing.Point(12, 393);
            this.AddButton.Name = "AddButton";
            this.AddButton.Size = new System.Drawing.Size(352, 44);
            this.AddButton.TabIndex = 1;
            this.AddButton.Text = "Добавить расписание";
            this.AddButton.UseVisualStyleBackColor = true;
            this.AddButton.Click += new System.EventHandler(this.AddButton_Click);
            // 
            // deleteButton
            // 
            this.deleteButton.Location = new System.Drawing.Point(370, 393);
            this.deleteButton.Name = "deleteButton";
            this.deleteButton.Size = new System.Drawing.Size(352, 44);
            this.deleteButton.TabIndex = 2;
            this.deleteButton.Text = "Удалить все расписания";
            this.deleteButton.UseVisualStyleBackColor = true;
            this.deleteButton.Click += new System.EventHandler(this.deleteButton_Click);
            // 
            // WorkForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1267, 449);
            this.Controls.Add(this.deleteButton);
            this.Controls.Add(this.AddButton);
            this.Controls.Add(this.dataGridView);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "WorkForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Полное расписание";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ViewForm_FormClosed);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridView;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn1;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn2;
        private TimeColumn startTime;
        private TimeColumn EndTime;
        private System.Windows.Forms.DataGridViewComboBoxColumn NumDevice;
        private System.Windows.Forms.DataGridViewCheckBoxColumn weekMonday;
        private System.Windows.Forms.DataGridViewCheckBoxColumn weekTuesday;
        private System.Windows.Forms.DataGridViewCheckBoxColumn weekWednesday;
        private System.Windows.Forms.DataGridViewCheckBoxColumn weekThursday;
        private System.Windows.Forms.DataGridViewCheckBoxColumn weekFriday;
        private System.Windows.Forms.DataGridViewCheckBoxColumn weekSaturday;
        private System.Windows.Forms.DataGridViewCheckBoxColumn weekSunday;
        private System.Windows.Forms.DataGridViewButtonColumn editRecord;
        private System.Windows.Forms.DataGridViewButtonColumn deleteRecord;
        private TimeColumn timeColumn1;
        private TimeColumn timeColumn2;
        private System.Windows.Forms.Button AddButton;
        private System.Windows.Forms.Button deleteButton;
    }
}