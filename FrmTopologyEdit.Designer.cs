
namespace UI
{
    partial class FrmTopologyEdit
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
            this.lblOperation = new System.Windows.Forms.Label();
            this.nudSewTolerance = new System.Windows.Forms.NumericUpDown();
            this.pnlFreeEdgeColor = new System.Windows.Forms.Panel();
            this.pnlSharedEdgeColor = new System.Windows.Forms.Panel();
            this.lblFreeEdge = new System.Windows.Forms.Label();
            this.lblSharedEdge = new System.Windows.Forms.Label();
            this.lblComponent = new System.Windows.Forms.Label();
            this.lblCurrentEdge = new System.Windows.Forms.Label();
            this.lblStatus = new System.Windows.Forms.Label();
            this.btnFinish = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.lblTorlerence = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.nudSewTolerance)).BeginInit();
            this.SuspendLayout();
            // 
            // lblOperation
            // 
            this.lblOperation.AutoSize = true;
            this.lblOperation.Location = new System.Drawing.Point(13, 13);
            this.lblOperation.Name = "lblOperation";
            this.lblOperation.Size = new System.Drawing.Size(221, 12);
            this.lblOperation.TabIndex = 0;
            this.lblOperation.Text = "单击红色自由边，自动寻找匹配边并缝合";
            // 
            // nudSewTolerance
            // 
            this.nudSewTolerance.DecimalPlaces = 4;
            this.nudSewTolerance.Increment = new decimal(new int[] {
            1,
            0,
            0,
            196608});
            this.nudSewTolerance.Location = new System.Drawing.Point(96, 51);
            this.nudSewTolerance.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.nudSewTolerance.Name = "nudSewTolerance";
            this.nudSewTolerance.Size = new System.Drawing.Size(120, 21);
            this.nudSewTolerance.TabIndex = 1;
            this.nudSewTolerance.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.nudSewTolerance.Value = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            // 
            // pnlFreeEdgeColor
            // 
            this.pnlFreeEdgeColor.BackColor = System.Drawing.Color.Red;
            this.pnlFreeEdgeColor.Location = new System.Drawing.Point(96, 94);
            this.pnlFreeEdgeColor.Name = "pnlFreeEdgeColor";
            this.pnlFreeEdgeColor.Size = new System.Drawing.Size(125, 10);
            this.pnlFreeEdgeColor.TabIndex = 2;
            // 
            // pnlSharedEdgeColor
            // 
            this.pnlSharedEdgeColor.BackColor = System.Drawing.Color.Lime;
            this.pnlSharedEdgeColor.Location = new System.Drawing.Point(96, 127);
            this.pnlSharedEdgeColor.Name = "pnlSharedEdgeColor";
            this.pnlSharedEdgeColor.Size = new System.Drawing.Size(125, 10);
            this.pnlSharedEdgeColor.TabIndex = 3;
            // 
            // lblFreeEdge
            // 
            this.lblFreeEdge.AutoSize = true;
            this.lblFreeEdge.Location = new System.Drawing.Point(13, 94);
            this.lblFreeEdge.Name = "lblFreeEdge";
            this.lblFreeEdge.Size = new System.Drawing.Size(77, 12);
            this.lblFreeEdge.TabIndex = 4;
            this.lblFreeEdge.Text = "红色：自由边";
            // 
            // lblSharedEdge
            // 
            this.lblSharedEdge.AutoSize = true;
            this.lblSharedEdge.Location = new System.Drawing.Point(13, 127);
            this.lblSharedEdge.Name = "lblSharedEdge";
            this.lblSharedEdge.Size = new System.Drawing.Size(77, 12);
            this.lblSharedEdge.TabIndex = 5;
            this.lblSharedEdge.Text = "绿色：共享边";
            // 
            // lblComponent
            // 
            this.lblComponent.AutoSize = true;
            this.lblComponent.Location = new System.Drawing.Point(15, 190);
            this.lblComponent.Name = "lblComponent";
            this.lblComponent.Size = new System.Drawing.Size(107, 12);
            this.lblComponent.TabIndex = 6;
            this.lblComponent.Text = "Component：未选择";
            // 
            // lblCurrentEdge
            // 
            this.lblCurrentEdge.AutoSize = true;
            this.lblCurrentEdge.Location = new System.Drawing.Point(13, 217);
            this.lblCurrentEdge.Name = "lblCurrentEdge";
            this.lblCurrentEdge.Size = new System.Drawing.Size(101, 12);
            this.lblCurrentEdge.TabIndex = 7;
            this.lblCurrentEdge.Text = "当前边：尚未选择";
            // 
            // lblStatus
            // 
            this.lblStatus.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblStatus.Location = new System.Drawing.Point(12, 248);
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(235, 70);
            this.lblStatus.TabIndex = 8;
            this.lblStatus.Text = "请选择一条红色自由边";
            // 
            // btnFinish
            // 
            this.btnFinish.Location = new System.Drawing.Point(38, 331);
            this.btnFinish.Name = "btnFinish";
            this.btnFinish.Size = new System.Drawing.Size(75, 23);
            this.btnFinish.TabIndex = 9;
            this.btnFinish.Text = "完成";
            this.btnFinish.UseVisualStyleBackColor = true;
            this.btnFinish.Click += new System.EventHandler(this.btnFinish_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(146, 331);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 10;
            this.btnCancel.Text = "取消";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // lblTorlerence
            // 
            this.lblTorlerence.AutoSize = true;
            this.lblTorlerence.Location = new System.Drawing.Point(15, 53);
            this.lblTorlerence.Name = "lblTorlerence";
            this.lblTorlerence.Size = new System.Drawing.Size(65, 12);
            this.lblTorlerence.TabIndex = 11;
            this.lblTorlerence.Text = "缝合容差：";
            // 
            // FrmTopologyEdit
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(264, 381);
            this.Controls.Add(this.lblTorlerence);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnFinish);
            this.Controls.Add(this.lblStatus);
            this.Controls.Add(this.lblCurrentEdge);
            this.Controls.Add(this.lblComponent);
            this.Controls.Add(this.lblSharedEdge);
            this.Controls.Add(this.lblFreeEdge);
            this.Controls.Add(this.pnlSharedEdgeColor);
            this.Controls.Add(this.pnlFreeEdgeColor);
            this.Controls.Add(this.nudSewTolerance);
            this.Controls.Add(this.lblOperation);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FrmTopologyEdit";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "TopologyEdit";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FrmTopologyEdit_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.nudSewTolerance)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblOperation;
        private System.Windows.Forms.NumericUpDown nudSewTolerance;
        private System.Windows.Forms.Panel pnlFreeEdgeColor;
        private System.Windows.Forms.Panel pnlSharedEdgeColor;
        private System.Windows.Forms.Label lblFreeEdge;
        private System.Windows.Forms.Label lblSharedEdge;
        private System.Windows.Forms.Label lblComponent;
        private System.Windows.Forms.Label lblCurrentEdge;
        private System.Windows.Forms.Label lblStatus;
        private System.Windows.Forms.Button btnFinish;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Label lblTorlerence;
    }
}