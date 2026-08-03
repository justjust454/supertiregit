
namespace UI
{
    partial class FrmMain
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmMain));
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.tsbNew = new System.Windows.Forms.ToolStripButton();
            this.btnDrawBox = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton2 = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.tsbDelete = new System.Windows.Forms.ToolStripButton();
            this.tsbAutoCombine = new System.Windows.Forms.ToolStripButton();
            this.tsbCombine = new System.Windows.Forms.ToolStripButton();
            this.tsbSplit = new System.Windows.Forms.ToolStripButton();
            this.tsbSurfaceCreate = new System.Windows.Forms.ToolStripButton();
            this.tsbCordCreate = new System.Windows.Forms.ToolStripButton();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.panel2D = new System.Windows.Forms.Panel();
            this.pnlFaceAction = new System.Windows.Forms.Panel();
            this.btnFaceCancel = new System.Windows.Forms.Button();
            this.btnFaceCreate = new System.Windows.Forms.Button();
            this.richTextBoxLog = new System.Windows.Forms.RichTextBox();
            this.toolStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.panel2D.SuspendLayout();
            this.pnlFaceAction.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsbNew,
            this.btnDrawBox,
            this.toolStripButton2,
            this.toolStripSeparator1,
            this.tsbDelete,
            this.tsbAutoCombine,
            this.tsbCombine,
            this.tsbSplit,
            this.tsbSurfaceCreate,
            this.tsbCordCreate});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(1071, 25);
            this.toolStrip1.TabIndex = 1;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // tsbNew
            // 
            this.tsbNew.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbNew.Image = ((System.Drawing.Image)(resources.GetObject("tsbNew.Image")));
            this.tsbNew.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbNew.Name = "tsbNew";
            this.tsbNew.Size = new System.Drawing.Size(23, 22);
            this.tsbNew.Text = "toolStripButton1";
            this.tsbNew.Click += new System.EventHandler(this.tsbNew_Click);
            // 
            // btnDrawBox
            // 
            this.btnDrawBox.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.btnDrawBox.Image = ((System.Drawing.Image)(resources.GetObject("btnDrawBox.Image")));
            this.btnDrawBox.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnDrawBox.Name = "btnDrawBox";
            this.btnDrawBox.Size = new System.Drawing.Size(23, 22);
            this.btnDrawBox.Text = "toolStripButton1";
            this.btnDrawBox.Click += new System.EventHandler(this.tsbImport_Click);
            // 
            // toolStripButton2
            // 
            this.toolStripButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton2.Image")));
            this.toolStripButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton2.Name = "toolStripButton2";
            this.toolStripButton2.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton2.Text = "toolStripButton2";
            this.toolStripButton2.Click += new System.EventHandler(this.tsbExport_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // tsbDelete
            // 
            this.tsbDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbDelete.Image = ((System.Drawing.Image)(resources.GetObject("tsbDelete.Image")));
            this.tsbDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbDelete.Name = "tsbDelete";
            this.tsbDelete.Size = new System.Drawing.Size(23, 22);
            this.tsbDelete.Text = "tsbDelete";
            this.tsbDelete.Click += new System.EventHandler(this.tsbDelete_Click);
            // 
            // tsbAutoCombine
            // 
            this.tsbAutoCombine.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbAutoCombine.Image = ((System.Drawing.Image)(resources.GetObject("tsbAutoCombine.Image")));
            this.tsbAutoCombine.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbAutoCombine.Name = "tsbAutoCombine";
            this.tsbAutoCombine.Size = new System.Drawing.Size(23, 22);
            this.tsbAutoCombine.Text = "Combine";
            this.tsbAutoCombine.Click += new System.EventHandler(this.tsbAutoCombine_Click);
            // 
            // tsbCombine
            // 
            this.tsbCombine.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbCombine.Image = ((System.Drawing.Image)(resources.GetObject("tsbCombine.Image")));
            this.tsbCombine.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbCombine.Name = "tsbCombine";
            this.tsbCombine.Size = new System.Drawing.Size(23, 22);
            this.tsbCombine.Text = "toolStripButton1";
            this.tsbCombine.Click += new System.EventHandler(this.tsbCombine_Click);
            // 
            // tsbSplit
            // 
            this.tsbSplit.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbSplit.Image = ((System.Drawing.Image)(resources.GetObject("tsbSplit.Image")));
            this.tsbSplit.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbSplit.Name = "tsbSplit";
            this.tsbSplit.Size = new System.Drawing.Size(23, 22);
            this.tsbSplit.Text = "tsbSplit";
            this.tsbSplit.Click += new System.EventHandler(this.tsbSplit_Click);
            // 
            // tsbSurfaceCreate
            // 
            this.tsbSurfaceCreate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbSurfaceCreate.Image = ((System.Drawing.Image)(resources.GetObject("tsbSurfaceCreate.Image")));
            this.tsbSurfaceCreate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbSurfaceCreate.Name = "tsbSurfaceCreate";
            this.tsbSurfaceCreate.Size = new System.Drawing.Size(23, 22);
            this.tsbSurfaceCreate.Text = "Surface Create";
            this.tsbSurfaceCreate.Click += new System.EventHandler(this.tsbSurfaceCreate_Click);
            // 
            // tsbCordCreate
            // 
            this.tsbCordCreate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbCordCreate.Image = ((System.Drawing.Image)(resources.GetObject("tsbCordCreate.Image")));
            this.tsbCordCreate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbCordCreate.Name = "tsbCordCreate";
            this.tsbCordCreate.Size = new System.Drawing.Size(23, 22);
            this.tsbCordCreate.Text = "CordCreate";
            this.tsbCordCreate.Click += new System.EventHandler(this.tsbCordCreate_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 25);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.treeView1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
            this.splitContainer1.Size = new System.Drawing.Size(1071, 522);
            this.splitContainer1.SplitterDistance = 125;
            this.splitContainer1.TabIndex = 2;
            // 
            // treeView1
            // 
            this.treeView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeView1.Location = new System.Drawing.Point(0, 0);
            this.treeView1.Name = "treeView1";
            this.treeView1.Size = new System.Drawing.Size(125, 522);
            this.treeView1.TabIndex = 0;
            this.treeView1.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView1_AfterSelect);
            this.treeView1.NodeMouseClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.treeView1_NodeMouseClick);
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.panel2D);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.richTextBoxLog);
            this.splitContainer2.Size = new System.Drawing.Size(942, 522);
            this.splitContainer2.SplitterDistance = 441;
            this.splitContainer2.TabIndex = 0;
            // 
            // panel2D
            // 
            this.panel2D.Controls.Add(this.pnlFaceAction);
            this.panel2D.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2D.Location = new System.Drawing.Point(0, 0);
            this.panel2D.Name = "panel2D";
            this.panel2D.Size = new System.Drawing.Size(942, 441);
            this.panel2D.TabIndex = 0;
            this.panel2D.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint_1);
            // 
            // pnlFaceAction
            // 
            this.pnlFaceAction.BackColor = System.Drawing.Color.White;
            this.pnlFaceAction.Controls.Add(this.btnFaceCancel);
            this.pnlFaceAction.Controls.Add(this.btnFaceCreate);
            this.pnlFaceAction.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.pnlFaceAction.Location = new System.Drawing.Point(0, 418);
            this.pnlFaceAction.Name = "pnlFaceAction";
            this.pnlFaceAction.Size = new System.Drawing.Size(942, 23);
            this.pnlFaceAction.TabIndex = 0;
            this.pnlFaceAction.Visible = false;
            // 
            // btnFaceCancel
            // 
            this.btnFaceCancel.Location = new System.Drawing.Point(93, 0);
            this.btnFaceCancel.Name = "btnFaceCancel";
            this.btnFaceCancel.Size = new System.Drawing.Size(75, 23);
            this.btnFaceCancel.TabIndex = 1;
            this.btnFaceCancel.Text = "Cancle";
            this.btnFaceCancel.UseVisualStyleBackColor = true;
            this.btnFaceCancel.Click += new System.EventHandler(this.btnFaceCancel_Click);
            // 
            // btnFaceCreate
            // 
            this.btnFaceCreate.Location = new System.Drawing.Point(0, 0);
            this.btnFaceCreate.Name = "btnFaceCreate";
            this.btnFaceCreate.Size = new System.Drawing.Size(75, 23);
            this.btnFaceCreate.TabIndex = 0;
            this.btnFaceCreate.Text = "Confirm";
            this.btnFaceCreate.UseVisualStyleBackColor = true;
            this.btnFaceCreate.Click += new System.EventHandler(this.btnFaceCreate_Click);
            // 
            // richTextBoxLog
            // 
            this.richTextBoxLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBoxLog.Location = new System.Drawing.Point(0, 0);
            this.richTextBoxLog.Name = "richTextBoxLog";
            this.richTextBoxLog.Size = new System.Drawing.Size(942, 77);
            this.richTextBoxLog.TabIndex = 0;
            this.richTextBoxLog.Text = "";
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1071, 547);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.toolStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "FrmMain";
            this.Text = "SuperTire_Preprocess";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.panel2D.ResumeLayout(false);
            this.pnlFaceAction.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton btnDrawBox;
        private System.Windows.Forms.ToolStripButton toolStripButton2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton tsbNew;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.Panel panel2D;
        private System.Windows.Forms.RichTextBox richTextBoxLog;
        private System.Windows.Forms.ToolStripButton tsbDelete;
        private System.Windows.Forms.ToolStripButton tsbAutoCombine;
        private System.Windows.Forms.ToolStripButton tsbSplit;
        private System.Windows.Forms.ToolStripButton tsbSurfaceCreate;
        private System.Windows.Forms.Panel pnlFaceAction;
        private System.Windows.Forms.Button btnFaceCancel;
        private System.Windows.Forms.Button btnFaceCreate;
        private System.Windows.Forms.ToolStripButton tsbCombine;
        private System.Windows.Forms.ToolStripButton tsbCordCreate;
    }
}

