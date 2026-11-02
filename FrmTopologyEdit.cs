using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UI
{
    public partial class FrmTopologyEdit : Form
    {
        public event EventHandler FinishRequested;
        public event EventHandler CancelRequested;
        public FrmTopologyEdit()
        {
            InitializeComponent();
        }
       
        /// <summary>
        /// 当前缝合容差
        /// </summary>
        public double Tolerance
        {
            get
            {
                return (double)nudSewTolerance.Value;
            }
            set
            {
                decimal newValue = Convert.ToDecimal(value);

                if (newValue < nudSewTolerance.Minimum)
                    newValue = nudSewTolerance.Minimum;

                if (newValue > nudSewTolerance.Maximum)
                    newValue = nudSewTolerance.Maximum;

                nudSewTolerance.Value = newValue;
            }
        }

        /// <summary>
        /// 当前正在编辑的 Component 名称
        /// </summary>
        public string ComponentName
        {
            get
            {
                return lblComponent.Text;
            }
            set
            {
                lblComponent.Text =
                    "Component：" + value;
            }
        }

        /// <summary>
        /// 当前拓扑边信息
        /// </summary>
        public string CurrentEdgeText
        {
            get
            {
                return lblCurrentEdge.Text;
            }
            set
            {
                lblCurrentEdge.Text =
                    "当前边：" + value;
            }
        }

        /// <summary>
        /// 操作状态信息
        /// </summary>
        public string StatusText
        {
            get
            {
                return lblStatus.Text;
            }
            set
            {
                lblStatus.Text = value;
            }
        }

        /// <summary>
        /// 非模态显示窗体，并放在主窗体右侧
        /// </summary>
        public void ShowFor(Form owner)
        {
            if (owner == null)
                return;

            Rectangle workingArea =
                Screen.FromControl(owner).WorkingArea;

            int targetX =
                owner.Right - Width - 20;

            int targetY =
                owner.Top + 80;

            targetX = Math.Max(
                workingArea.Left,
                Math.Min(
                    targetX,
                    workingArea.Right - Width));

            targetY = Math.Max(
                workingArea.Top,
                Math.Min(
                    targetY,
                    workingArea.Bottom - Height));

            Location = new Point(
                targetX,
                targetY);

            if (!Visible)
            {
                // 必须使用 Show，不能使用 ShowDialog
                Show(owner);
            }
            else
            {
                BringToFront();
            }
        }

        public void HideToolWindow()
        {
            Hide();
        }

        private void btnFinish_Click(object sender, EventArgs e)
        {
            FinishRequested?.Invoke(
               this,
               EventArgs.Empty);
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            CancelRequested?.Invoke(
                this,
                EventArgs.Empty);
        }

        private void FrmTopologyEdit_FormClosing(object sender, FormClosingEventArgs e)
        {
            // 点击右上角 X 时不要销毁窗体，
            // 否则下一次进入拓扑模式需要重新创建
            if (e.CloseReason ==
                CloseReason.UserClosing)
            {
                e.Cancel = true;

                CancelRequested?.Invoke(
                    this,
                    EventArgs.Empty);
            }
        }
    }
}
