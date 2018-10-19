namespace MPS
{
    partial class ViewForm
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
            this.startTime = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.EndTime = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.NumDevice = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.weekMonday = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.weekTuesday = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.weekWednesday = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.weekThursday = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.weekFriday = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.weekSaturday = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.weekSunday = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.editRecord = new System.Windows.Forms.DataGridViewButtonColumn();
            this.deleteRecord = new System.Windows.Forms.DataGridViewButtonColumn();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGridView
            // 
            this.dataGridView.AllowUserToAddRows = false;
            this.dataGridView.AllowUserToDeleteRows = false;
            this.dataGridView.AllowUserToResizeColumns = false;
            this.dataGridView.AllowUserToResizeRows = false;
            this.dataGridView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
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
            this.dataGridView.Location = new System.Drawing.Point(12, 12);
            this.dataGridView.Name = "dataGridView";
            this.dataGridView.ReadOnly = true;
            this.dataGridView.Size = new System.Drawing.Size(1044, 443);
            this.dataGridView.TabIndex = 0;
            // 
            // startTime
            // 
            this.startTime.HeaderText = "Начальное время";
            this.startTime.Name = "startTime";
            this.startTime.ReadOnly = true;
            // 
            // EndTime
            // 
            this.EndTime.HeaderText = "Конечное время";
            this.EndTime.Name = "EndTime";
            this.EndTime.ReadOnly = true;
            // 
            // NumDevice
            // 
            this.NumDevice.HeaderText = "Номер реле";
            this.NumDevice.Name = "NumDevice";
            this.NumDevice.ReadOnly = true;
            // 
            // weekMonday
            // 
            this.weekMonday.HeaderText = "Понедельник";
            this.weekMonday.Name = "weekMonday";
            this.weekMonday.ReadOnly = true;
            // 
            // weekTuesday
            // 
            this.weekTuesday.HeaderText = "Вторник";
            this.weekTuesday.Name = "weekTuesday";
            this.weekTuesday.ReadOnly = true;
            this.weekTuesday.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // weekWednesday
            // 
            this.weekWednesday.HeaderText = "Среда";
            this.weekWednesday.Name = "weekWednesday";
            this.weekWednesday.ReadOnly = true;
            // 
            // weekThursday
            // 
            this.weekThursday.HeaderText = "Четверг";
            this.weekThursday.Name = "weekThursday";
            this.weekThursday.ReadOnly = true;
            // 
            // weekFriday
            // 
            this.weekFriday.HeaderText = "Пятница";
            this.weekFriday.Name = "weekFriday";
            this.weekFriday.ReadOnly = true;
            // 
            // weekSaturday
            // 
            this.weekSaturday.HeaderText = "Суббота";
            this.weekSaturday.Name = "weekSaturday";
            this.weekSaturday.ReadOnly = true;
            this.weekSaturday.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // weekSunday
            // 
            this.weekSunday.HeaderText = "Воскресенье";
            this.weekSunday.Name = "weekSunday";
            this.weekSunday.ReadOnly = true;
            // 
            // editRecord
            // 
            this.editRecord.HeaderText = "Изменение";
            this.editRecord.Name = "editRecord";
            this.editRecord.ReadOnly = true;
            this.editRecord.Text = "Изменить";
            // 
            // deleteRecord
            // 
            this.deleteRecord.HeaderText = "Удаление";
            this.deleteRecord.Name = "deleteRecord";
            this.deleteRecord.ReadOnly = true;
            this.deleteRecord.Text = "Удалить";
            // 
            // ViewForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1067, 465);
            this.Controls.Add(this.dataGridView);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "ViewForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Полное расписание";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ViewForm_FormClosed);
            this.Load += new System.EventHandler(this.ViewForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridView;
        private System.Windows.Forms.DataGridViewTextBoxColumn startTime;
        private System.Windows.Forms.DataGridViewTextBoxColumn EndTime;
        private System.Windows.Forms.DataGridViewTextBoxColumn NumDevice;
        private System.Windows.Forms.DataGridViewCheckBoxColumn weekMonday;
        private System.Windows.Forms.DataGridViewCheckBoxColumn weekTuesday;
        private System.Windows.Forms.DataGridViewCheckBoxColumn weekWednesday;
        private System.Windows.Forms.DataGridViewCheckBoxColumn weekThursday;
        private System.Windows.Forms.DataGridViewCheckBoxColumn weekFriday;
        private System.Windows.Forms.DataGridViewCheckBoxColumn weekSaturday;
        private System.Windows.Forms.DataGridViewCheckBoxColumn weekSunday;
        private System.Windows.Forms.DataGridViewButtonColumn editRecord;
        private System.Windows.Forms.DataGridViewButtonColumn deleteRecord;
    }
}