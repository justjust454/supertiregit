namespace UI
{
    partial class FrmParametricTireInput
    {
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.Label lblHeader;
        private System.Windows.Forms.TabControl tabMain;
        private System.Windows.Forms.TabPage tabDimensions;
        private System.Windows.Forms.TabPage tabAdjust;
        private System.Windows.Forms.TabPage tabThickness;
        private System.Windows.Forms.TabPage tabPoints;
        private System.Windows.Forms.TableLayoutPanel layoutDimensions;
        private System.Windows.Forms.TableLayoutPanel layoutAdjust;
        private System.Windows.Forms.TableLayoutPanel layoutThickness;
        private System.Windows.Forms.NumericUpDown nudCrownHeight;
        private System.Windows.Forms.NumericUpDown nudHalfWidth;
        private System.Windows.Forms.NumericUpDown nudBeadCenterX;
        private System.Windows.Forms.NumericUpDown nudBeadCenterY;
        private System.Windows.Forms.NumericUpDown nudBeadRadius;
        private System.Windows.Forms.NumericUpDown nudCrownDx;
        private System.Windows.Forms.NumericUpDown nudCrownDy;
        private System.Windows.Forms.NumericUpDown nudShoulderDx;
        private System.Windows.Forms.NumericUpDown nudShoulderDy;
        private System.Windows.Forms.NumericUpDown nudSidewallDx;
        private System.Windows.Forms.NumericUpDown nudSidewallDy;
        private System.Windows.Forms.NumericUpDown nudBeadDx;
        private System.Windows.Forms.NumericUpDown nudBeadDy;
        private System.Windows.Forms.NumericUpDown nudTread;
        private System.Windows.Forms.NumericUpDown nudBuffer;
        private System.Windows.Forms.NumericUpDown nudCarcass;
        private System.Windows.Forms.NumericUpDown nudInnerLiner;
        private System.Windows.Forms.NumericUpDown nudSidewall;
        private System.Windows.Forms.NumericUpDown nudBeadHeel;
        private System.Windows.Forms.CheckBox chkBuildFace;
        private System.Windows.Forms.CheckBox chkMirror;
        private System.Windows.Forms.DataGridView gridPoints;
        private System.Windows.Forms.DataGridViewTextBoxColumn colIndex;
        private System.Windows.Forms.DataGridViewTextBoxColumn colZone;
        private System.Windows.Forms.DataGridViewTextBoxColumn colBaseX;
        private System.Windows.Forms.DataGridViewTextBoxColumn colBaseY;
        private System.Windows.Forms.DataGridViewTextBoxColumn colDeltaX;
        private System.Windows.Forms.DataGridViewTextBoxColumn colDeltaY;
        private System.Windows.Forms.DataGridViewCheckBoxColumn colLocked;
        private System.Windows.Forms.Panel panelFooter;
        private System.Windows.Forms.Label lblStatus;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnGenerate;
        private System.Windows.Forms.Button btnPreview;
        private System.Windows.Forms.Button btnReset;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
                components.Dispose();
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.lblHeader = new System.Windows.Forms.Label();
            this.tabMain = new System.Windows.Forms.TabControl();
            this.tabDimensions = new System.Windows.Forms.TabPage();
            this.layoutDimensions = new System.Windows.Forms.TableLayoutPanel();
            this.tabAdjust = new System.Windows.Forms.TabPage();
            this.layoutAdjust = new System.Windows.Forms.TableLayoutPanel();
            this.tabThickness = new System.Windows.Forms.TabPage();
            this.layoutThickness = new System.Windows.Forms.TableLayoutPanel();
            this.tabPoints = new System.Windows.Forms.TabPage();
            this.gridPoints = new System.Windows.Forms.DataGridView();
            this.colIndex = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colZone = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colBaseX = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colBaseY = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colDeltaX = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colDeltaY = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.colLocked = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.panelFooter = new System.Windows.Forms.Panel();
            this.lblStatus = new System.Windows.Forms.Label();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnGenerate = new System.Windows.Forms.Button();
            this.btnPreview = new System.Windows.Forms.Button();
            this.btnReset = new System.Windows.Forms.Button();
            this.nudCrownHeight = CreateNumber(1.0m, 5000.0m, 2, 10.0m);
            this.nudHalfWidth = CreateNumber(1.0m, 2000.0m, 2, 1.0m);
            this.nudBeadCenterX = CreateNumber(-1000.0m, 2000.0m, 3, 0.1m);
            this.nudBeadCenterY = CreateNumber(-1000.0m, 2000.0m, 3, 0.1m);
            this.nudBeadRadius = CreateNumber(0.1m, 500.0m, 3, 0.1m);
            this.nudCrownDx = CreateNumber(-500.0m, 500.0m, 2, 0.5m);
            this.nudCrownDy = CreateNumber(-500.0m, 500.0m, 2, 0.5m);
            this.nudShoulderDx = CreateNumber(-500.0m, 500.0m, 2, 0.5m);
            this.nudShoulderDy = CreateNumber(-500.0m, 500.0m, 2, 0.5m);
            this.nudSidewallDx = CreateNumber(-500.0m, 500.0m, 2, 0.5m);
            this.nudSidewallDy = CreateNumber(-500.0m, 500.0m, 2, 0.5m);
            this.nudBeadDx = CreateNumber(-500.0m, 500.0m, 2, 0.5m);
            this.nudBeadDy = CreateNumber(-500.0m, 500.0m, 2, 0.5m);
            this.nudTread = CreateNumber(0.1m, 200.0m, 2, 0.5m);
            this.nudBuffer = CreateNumber(0.1m, 200.0m, 2, 0.5m);
            this.nudCarcass = CreateNumber(0.1m, 200.0m, 2, 0.5m);
            this.nudInnerLiner = CreateNumber(0.1m, 200.0m, 2, 0.5m);
            this.nudSidewall = CreateNumber(0.1m, 200.0m, 2, 0.5m);
            this.nudBeadHeel = CreateNumber(0.1m, 200.0m, 2, 0.5m);
            this.chkBuildFace = new System.Windows.Forms.CheckBox();
            this.chkMirror = new System.Windows.Forms.CheckBox();

            this.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.gridPoints)).BeginInit();

            this.lblHeader.BackColor = System.Drawing.Color.FromArgb(31, 78, 121);
            this.lblHeader.Dock = System.Windows.Forms.DockStyle.Top;
            this.lblHeader.Font = new System.Drawing.Font(
                "Microsoft YaHei UI", 15F, System.Drawing.FontStyle.Bold);
            this.lblHeader.ForeColor = System.Drawing.Color.White;
            this.lblHeader.Location = new System.Drawing.Point(0, 0);
            this.lblHeader.Name = "lblHeader";
            this.lblHeader.Padding = new System.Windows.Forms.Padding(18, 0, 0, 0);
            this.lblHeader.Size = new System.Drawing.Size(1040, 52);
            this.lblHeader.TabIndex = 0;
            this.lblHeader.Text = "参数输入界面";
            this.lblHeader.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;

            this.tabMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabMain.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F);
            this.tabMain.Location = new System.Drawing.Point(0, 52);
            this.tabMain.Name = "tabMain";
            this.tabMain.SelectedIndex = 0;
            this.tabMain.Size = new System.Drawing.Size(1040, 590);
            this.tabMain.TabIndex = 1;
            this.tabMain.TabPages.AddRange(new System.Windows.Forms.TabPage[] {
                this.tabDimensions,
                this.tabAdjust,
                this.tabThickness,
                this.tabPoints});

            this.tabDimensions.Controls.Add(this.layoutDimensions);
            this.tabDimensions.Location = new System.Drawing.Point(4, 26);
            this.tabDimensions.Name = "tabDimensions";
            this.tabDimensions.Padding = new System.Windows.Forms.Padding(3);
            this.tabDimensions.Size = new System.Drawing.Size(1032, 560);
            this.tabDimensions.TabIndex = 0;
            this.tabDimensions.Text = "基本尺寸";
            this.tabDimensions.UseVisualStyleBackColor = true;

            ConfigureTable(this.layoutDimensions, 2);
            this.layoutDimensions.Controls.Add(CreateLabel("胎冠半径 / 最大高度 Y (mm)："), 0, 0);
            this.layoutDimensions.Controls.Add(this.nudCrownHeight, 1, 0);
            this.layoutDimensions.Controls.Add(CreateLabel("轮胎半宽 X (mm)："), 0, 1);
            this.layoutDimensions.Controls.Add(this.nudHalfWidth, 1, 1);
            this.layoutDimensions.Controls.Add(CreateLabel("钢丝圈中心 X (mm)："), 0, 2);
            this.layoutDimensions.Controls.Add(this.nudBeadCenterX, 1, 2);
            this.layoutDimensions.Controls.Add(CreateLabel("钢丝圈中心 Y (mm)："), 0, 3);
            this.layoutDimensions.Controls.Add(this.nudBeadCenterY, 1, 3);
            this.layoutDimensions.Controls.Add(CreateLabel("钢丝圈半径 (mm)："), 0, 4);
            this.layoutDimensions.Controls.Add(this.nudBeadRadius, 1, 4);
            System.Windows.Forms.Label dimensionsNote = CreateLabel(
                "基准坐标来自建模手册图 1-2 至图 1-5。修改高度或半宽会按比例调整全部关键点；\r\n" +
                "材料厚度保持绝对毫米值，不随外形比例缩放。");
            dimensionsNote.ForeColor = System.Drawing.Color.DimGray;
            dimensionsNote.AutoSize = true;
            this.layoutDimensions.Controls.Add(dimensionsNote, 0, 5);
            this.layoutDimensions.SetColumnSpan(dimensionsNote, 2);

            this.tabAdjust.Controls.Add(this.layoutAdjust);
            this.tabAdjust.Location = new System.Drawing.Point(4, 26);
            this.tabAdjust.Name = "tabAdjust";
            this.tabAdjust.Padding = new System.Windows.Forms.Padding(3);
            this.tabAdjust.Size = new System.Drawing.Size(1032, 560);
            this.tabAdjust.TabIndex = 1;
            this.tabAdjust.Text = "外轮廓微调";
            this.tabAdjust.UseVisualStyleBackColor = true;

            ConfigureTable(this.layoutAdjust, 4);
            this.layoutAdjust.Controls.Add(CreateHeader("控制区域"), 0, 0);
            this.layoutAdjust.Controls.Add(CreateHeader("X 位移 (mm)"), 1, 0);
            this.layoutAdjust.Controls.Add(CreateHeader("Y 位移 (mm)"), 2, 0);
            this.layoutAdjust.Controls.Add(CreateHeader("作用"), 3, 0);
            AddAdjustmentRow(this.layoutAdjust, 1, "胎冠", this.nudCrownDx, this.nudCrownDy, "调整胎冠中心与顶部平台");
            AddAdjustmentRow(this.layoutAdjust, 2, "胎肩", this.nudShoulderDx, this.nudShoulderDy, "调整肩部过渡位置");
            AddAdjustmentRow(this.layoutAdjust, 3, "胎侧", this.nudSidewallDx, this.nudSidewallDy, "调整最大宽度与侧壁弧度");
            AddAdjustmentRow(this.layoutAdjust, 4, "胎圈", this.nudBeadDx, this.nudBeadDy, "调整钢丝圈与胎踵区域");
            System.Windows.Forms.Label adjustNote = CreateLabel(
                "位移采用距离加权，影响会平滑衰减，邻接曲线共用同一关键点，因此不会产生端点裂缝。\r\n" +
                "需要逐点控制时，请使用“高级点表”。");
            adjustNote.ForeColor = System.Drawing.Color.DimGray;
            adjustNote.AutoSize = true;
            this.layoutAdjust.Controls.Add(adjustNote, 0, 5);
            this.layoutAdjust.SetColumnSpan(adjustNote, 4);

            this.tabThickness.Controls.Add(this.layoutThickness);
            this.tabThickness.Location = new System.Drawing.Point(4, 26);
            this.tabThickness.Name = "tabThickness";
            this.tabThickness.Padding = new System.Windows.Forms.Padding(3);
            this.tabThickness.Size = new System.Drawing.Size(1032, 560);
            this.tabThickness.TabIndex = 2;
            this.tabThickness.Text = "内部层厚";
            this.tabThickness.UseVisualStyleBackColor = true;

            ConfigureTable(this.layoutThickness, 2);
            AddThicknessRow(this.layoutThickness, 0, "胎面厚度 (mm)：", this.nudTread);
            AddThicknessRow(this.layoutThickness, 1, "缓冲层厚度 (mm)：", this.nudBuffer);
            AddThicknessRow(this.layoutThickness, 2, "胎基厚度 (mm)：", this.nudCarcass);
            AddThicknessRow(this.layoutThickness, 3, "气密层厚度 (mm)：", this.nudInnerLiner);
            AddThicknessRow(this.layoutThickness, 4, "胎侧厚度 (mm)：", this.nudSidewall);
            AddThicknessRow(this.layoutThickness, 5, "胎踵厚度 (mm)：", this.nudBeadHeel);
            this.chkBuildFace.AutoSize = true;
            this.chkBuildFace.Checked = true;
            this.chkBuildFace.Text = "生成轮胎截面面";
            this.layoutThickness.Controls.Add(this.chkBuildFace, 0, 6);
            this.layoutThickness.SetColumnSpan(this.chkBuildFace, 2);
            this.chkMirror.AutoSize = true;
            this.chkMirror.Checked = true;
            this.chkMirror.Text = "沿 X=0 镜像生成完整二维截面";
            this.layoutThickness.Controls.Add(this.chkMirror, 0, 7);
            this.layoutThickness.SetColumnSpan(this.chkMirror, 2);
            System.Windows.Forms.Label thicknessNote = CreateLabel(
                "顶部内层由气密层内轮廓向外依次累加：气密层 2、胎基 7、缓冲层 19、胎面 10。\r\n" +
                "胎肩、胎侧和胎踵分别裁剪，禁止跨越胎肩小圆弧连续累计偏置。");
            thicknessNote.ForeColor = System.Drawing.Color.DimGray;
            thicknessNote.AutoSize = true;
            this.layoutThickness.Controls.Add(thicknessNote, 0, 8);
            this.layoutThickness.SetColumnSpan(thicknessNote, 2);

            this.tabPoints.Controls.Add(this.gridPoints);
            this.tabPoints.Location = new System.Drawing.Point(4, 26);
            this.tabPoints.Name = "tabPoints";
            this.tabPoints.Padding = new System.Windows.Forms.Padding(3);
            this.tabPoints.Size = new System.Drawing.Size(1032, 560);
            this.tabPoints.TabIndex = 3;
            this.tabPoints.Text = "高级点表";
            this.tabPoints.UseVisualStyleBackColor = true;

            this.gridPoints.AllowUserToAddRows = false;
            this.gridPoints.AllowUserToDeleteRows = false;
            this.gridPoints.AutoGenerateColumns = false;
            this.gridPoints.ColumnHeadersHeightSizeMode =
                System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.gridPoints.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
                this.colIndex,
                this.colZone,
                this.colBaseX,
                this.colBaseY,
                this.colDeltaX,
                this.colDeltaY,
                this.colLocked});
            this.gridPoints.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gridPoints.Location = new System.Drawing.Point(3, 3);
            this.gridPoints.MultiSelect = false;
            this.gridPoints.Name = "gridPoints";
            this.gridPoints.RowHeadersVisible = false;
            this.gridPoints.RowTemplate.Height = 24;
            this.gridPoints.SelectionMode =
                System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.gridPoints.Size = new System.Drawing.Size(1026, 554);
            this.gridPoints.TabIndex = 0;

            this.colIndex.DataPropertyName = "Index";
            this.colIndex.HeaderText = "编号";
            this.colIndex.Name = "colIndex";
            this.colIndex.ReadOnly = true;
            this.colIndex.Width = 55;
            this.colZone.DataPropertyName = "Zone";
            this.colZone.HeaderText = "区域";
            this.colZone.Name = "colZone";
            this.colZone.ReadOnly = true;
            this.colZone.Width = 150;
            this.colBaseX.DataPropertyName = "BaseX";
            this.colBaseX.HeaderText = "基准 X";
            this.colBaseX.Name = "colBaseX";
            this.colBaseX.Width = 110;
            this.colBaseY.DataPropertyName = "BaseY";
            this.colBaseY.HeaderText = "基准 Y";
            this.colBaseY.Name = "colBaseY";
            this.colBaseY.Width = 110;
            this.colDeltaX.DataPropertyName = "DeltaX";
            this.colDeltaX.HeaderText = "局部 ΔX";
            this.colDeltaX.Name = "colDeltaX";
            this.colDeltaX.Width = 110;
            this.colDeltaY.DataPropertyName = "DeltaY";
            this.colDeltaY.HeaderText = "局部 ΔY";
            this.colDeltaY.Name = "colDeltaY";
            this.colDeltaY.Width = 110;
            this.colLocked.DataPropertyName = "Locked";
            this.colLocked.HeaderText = "锁定";
            this.colLocked.Name = "colLocked";
            this.colLocked.Width = 70;

            this.panelFooter.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panelFooter.Location = new System.Drawing.Point(0, 642);
            this.panelFooter.Name = "panelFooter";
            this.panelFooter.Padding = new System.Windows.Forms.Padding(12, 8, 12, 8);
            this.panelFooter.Size = new System.Drawing.Size(1040, 58);
            this.panelFooter.TabIndex = 2;

            this.lblStatus.AutoEllipsis = true;
            this.lblStatus.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblStatus.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F);
            this.lblStatus.ForeColor = System.Drawing.Color.DimGray;
            this.lblStatus.Location = new System.Drawing.Point(12, 8);
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(570, 42);
            this.lblStatus.TabIndex = 0;
            this.lblStatus.Text = "设置参数后可先预览；预览不会写入 GlobalShapeMap。";
            this.lblStatus.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;

            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(930, 13);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(88, 32);
            this.btnCancel.TabIndex = 4;
            this.btnCancel.Text = "取消";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);

            this.btnGenerate.Location = new System.Drawing.Point(828, 13);
            this.btnGenerate.Name = "btnGenerate";
            this.btnGenerate.Size = new System.Drawing.Size(92, 32);
            this.btnGenerate.TabIndex = 3;
            this.btnGenerate.Text = "生成";
            this.btnGenerate.UseVisualStyleBackColor = true;
            this.btnGenerate.Click += new System.EventHandler(this.btnGenerate_Click);

            this.btnPreview.Location = new System.Drawing.Point(718, 13);
            this.btnPreview.Name = "btnPreview";
            this.btnPreview.Size = new System.Drawing.Size(100, 32);
            this.btnPreview.TabIndex = 2;
            this.btnPreview.Text = "预览";
            this.btnPreview.UseVisualStyleBackColor = true;
            this.btnPreview.Click += new System.EventHandler(this.btnPreview_Click);

            this.btnReset.Location = new System.Drawing.Point(608, 13);
            this.btnReset.Name = "btnReset";
            this.btnReset.Size = new System.Drawing.Size(100, 32);
            this.btnReset.TabIndex = 1;
            this.btnReset.Text = "恢复基准";
            this.btnReset.UseVisualStyleBackColor = true;
            this.btnReset.Click += new System.EventHandler(this.btnReset_Click);

            this.panelFooter.Controls.Add(this.lblStatus);
            this.panelFooter.Controls.Add(this.btnReset);
            this.panelFooter.Controls.Add(this.btnPreview);
            this.panelFooter.Controls.Add(this.btnGenerate);
            this.panelFooter.Controls.Add(this.btnCancel);

            this.AcceptButton = this.btnGenerate;
            this.CancelButton = this.btnCancel;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1040, 700);
            this.Controls.Add(this.tabMain);
            this.Controls.Add(this.panelFooter);
            this.Controls.Add(this.lblHeader);
            this.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F);
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(900, 650);
            this.Name = "FrmParametricTireInput";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "参数输入界面";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(
                this.FrmParametricTireInput_FormClosing);

            ((System.ComponentModel.ISupportInitialize)(this.gridPoints)).EndInit();
            this.ResumeLayout(false);
        }

        private static System.Windows.Forms.NumericUpDown CreateNumber(
            decimal minimum,
            decimal maximum,
            int decimalPlaces,
            decimal increment)
        {
            System.Windows.Forms.NumericUpDown control =
                new System.Windows.Forms.NumericUpDown();
            control.Minimum = minimum;
            control.Maximum = maximum;
            control.DecimalPlaces = decimalPlaces;
            control.Increment = increment;
            control.Width = 210;
            control.Anchor = System.Windows.Forms.AnchorStyles.Left
                | System.Windows.Forms.AnchorStyles.Right;
            control.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            return control;
        }

        private static System.Windows.Forms.Label CreateLabel(string text)
        {
            return new System.Windows.Forms.Label
            {
                Text = text,
                AutoSize = true,
                Padding = new System.Windows.Forms.Padding(0, 8, 0, 8)
            };
        }

        private static System.Windows.Forms.Label CreateHeader(string text)
        {
            return new System.Windows.Forms.Label
            {
                Text = text,
                AutoSize = true,
                Font = new System.Drawing.Font(
                    "Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold),
                Padding = new System.Windows.Forms.Padding(0, 8, 0, 8)
            };
        }

        private static void ConfigureTable(
            System.Windows.Forms.TableLayoutPanel table,
            int columnCount)
        {
            table.ColumnCount = columnCount;
            table.Dock = System.Windows.Forms.DockStyle.Fill;
            table.Padding = new System.Windows.Forms.Padding(24);
            table.AutoSize = true;

            if (columnCount == 2)
            {
                table.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(
                    System.Windows.Forms.SizeType.Percent, 45F));
                table.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(
                    System.Windows.Forms.SizeType.Percent, 55F));
            }
            else
            {
                table.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(
                    System.Windows.Forms.SizeType.Percent, 14F));
                table.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(
                    System.Windows.Forms.SizeType.Percent, 22F));
                table.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(
                    System.Windows.Forms.SizeType.Percent, 22F));
                table.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(
                    System.Windows.Forms.SizeType.Percent, 42F));
            }
        }

        private static void AddAdjustmentRow(
            System.Windows.Forms.TableLayoutPanel table,
            int row,
            string name,
            System.Windows.Forms.NumericUpDown dx,
            System.Windows.Forms.NumericUpDown dy,
            string description)
        {
            table.Controls.Add(CreateLabel(name), 0, row);
            table.Controls.Add(dx, 1, row);
            table.Controls.Add(dy, 2, row);
            table.Controls.Add(CreateLabel(description), 3, row);
        }

        private static void AddThicknessRow(
            System.Windows.Forms.TableLayoutPanel table,
            int row,
            string name,
            System.Windows.Forms.NumericUpDown control)
        {
            table.Controls.Add(CreateLabel(name), 0, row);
            table.Controls.Add(control, 1, row);
        }
    }
}
