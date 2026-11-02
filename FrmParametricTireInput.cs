using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace UI
{
    public partial class FrmParametricTireInput : Form
    {
        private TireSectionParameterSet parameters;
        private bool generationCommitted;
        private bool suppressCancelEvent;

        public event EventHandler<ParametricTireRequestEventArgs> PreviewRequested;
        public event EventHandler<ParametricTireRequestEventArgs> GenerateRequested;
        public event EventHandler CancelRequested;

        public FrmParametricTireInput()
        {
            InitializeComponent();
            parameters = TireSectionParameterSet.CreateDefault();
            LoadParameters(parameters);
        }

        private void LoadParameters(TireSectionParameterSet source)
        {
            parameters = source.Clone();

            nudCrownHeight.Value = ToDecimal(parameters.CrownHeight);
            nudHalfWidth.Value = ToDecimal(parameters.HalfWidth);
            nudBeadCenterX.Value = ToDecimal(parameters.BeadCenterX);
            nudBeadCenterY.Value = ToDecimal(parameters.BeadCenterY);
            nudBeadRadius.Value = ToDecimal(parameters.BeadRadius);

            nudCrownDx.Value = ToDecimal(parameters.CrownDx);
            nudCrownDy.Value = ToDecimal(parameters.CrownDy);
            nudShoulderDx.Value = ToDecimal(parameters.ShoulderDx);
            nudShoulderDy.Value = ToDecimal(parameters.ShoulderDy);
            nudSidewallDx.Value = ToDecimal(parameters.SidewallDx);
            nudSidewallDy.Value = ToDecimal(parameters.SidewallDy);
            nudBeadDx.Value = ToDecimal(parameters.BeadDx);
            nudBeadDy.Value = ToDecimal(parameters.BeadDy);

            nudTread.Value = ToDecimal(parameters.TreadThickness);
            nudBuffer.Value = ToDecimal(parameters.BufferThickness);
            nudCarcass.Value = ToDecimal(parameters.CarcassThickness);
            nudInnerLiner.Value = ToDecimal(parameters.InnerLinerThickness);
            nudSidewall.Value = ToDecimal(parameters.SidewallThickness);
            nudBeadHeel.Value = ToDecimal(parameters.BeadHeelThickness);

            chkBuildFace.Checked = parameters.BuildSectionFace;
            chkMirror.Checked = parameters.MirrorSection;

            gridPoints.DataSource = null;
            gridPoints.DataSource = new BindingList<TireSectionPoint>(
                parameters.Points);
            gridPoints.AutoResizeColumns(
                DataGridViewAutoSizeColumnsMode.AllCells);

            ShowStatus("已载入手册默认尺寸。", Color.DimGray);
        }

        private TireSectionParameterSet CollectParameters()
        {
            gridPoints.EndEdit();

            parameters.CrownHeight = ToDouble(nudCrownHeight.Value);
            parameters.HalfWidth = ToDouble(nudHalfWidth.Value);
            parameters.BeadCenterX = ToDouble(nudBeadCenterX.Value);
            parameters.BeadCenterY = ToDouble(nudBeadCenterY.Value);
            parameters.BeadRadius = ToDouble(nudBeadRadius.Value);

            parameters.CrownDx = ToDouble(nudCrownDx.Value);
            parameters.CrownDy = ToDouble(nudCrownDy.Value);
            parameters.ShoulderDx = ToDouble(nudShoulderDx.Value);
            parameters.ShoulderDy = ToDouble(nudShoulderDy.Value);
            parameters.SidewallDx = ToDouble(nudSidewallDx.Value);
            parameters.SidewallDy = ToDouble(nudSidewallDy.Value);
            parameters.BeadDx = ToDouble(nudBeadDx.Value);
            parameters.BeadDy = ToDouble(nudBeadDy.Value);

            parameters.TreadThickness = ToDouble(nudTread.Value);
            parameters.BufferThickness = ToDouble(nudBuffer.Value);
            parameters.CarcassThickness = ToDouble(nudCarcass.Value);
            parameters.InnerLinerThickness = ToDouble(nudInnerLiner.Value);
            parameters.SidewallThickness = ToDouble(nudSidewall.Value);
            parameters.BeadHeelThickness = ToDouble(nudBeadHeel.Value);

            parameters.BuildSectionFace = chkBuildFace.Checked;
            parameters.MirrorSection = chkMirror.Checked;
            return parameters.Clone();
        }

        private void btnReset_Click(object sender, EventArgs e)
        {
            CancelPreview();
            LoadParameters(TireSectionParameterSet.CreateDefault());
        }

        private void btnPreview_Click(object sender, EventArgs e)
        {
            TireSectionParameterSet current = CollectParameters();
            string error = current.Validate();
            if (!string.IsNullOrEmpty(error))
            {
                ShowStatus(error, Color.Firebrick);
                MessageBox.Show(this, error, "参数检查",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            ParametricTireRequestEventArgs args =
                new ParametricTireRequestEventArgs(current);
            if (PreviewRequested != null)
                PreviewRequested(this, args);

            if (args.Success)
                ShowStatus("预览已生成，尚未写入 GlobalShapeMap。", Color.SeaGreen);
            else
                ShowStatus(
                    string.IsNullOrEmpty(args.Message) ? "预览失败。" : args.Message,
                    Color.Firebrick);
        }

        private void btnGenerate_Click(object sender, EventArgs e)
        {
            TireSectionParameterSet current = CollectParameters();
            string error = current.Validate();
            if (!string.IsNullOrEmpty(error))
            {
                ShowStatus(error, Color.Firebrick);
                MessageBox.Show(this, error, "参数检查",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            ParametricTireRequestEventArgs args =
                new ParametricTireRequestEventArgs(current);
            if (GenerateRequested != null)
                GenerateRequested(this, args);

            if (!args.Success)
            {
                ShowStatus(
                    string.IsNullOrEmpty(args.Message) ? "生成失败。" : args.Message,
                    Color.Firebrick);
                return;
            }

            generationCommitted = true;
            DialogResult = DialogResult.OK;
            Close();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void FrmParametricTireInput_FormClosing(
            object sender,
            FormClosingEventArgs e)
        {
            if (!generationCommitted && !suppressCancelEvent)
            {
                suppressCancelEvent = true;
                CancelPreview();
            }
        }

        private void CancelPreview()
        {
            if (CancelRequested != null)
                CancelRequested(this, EventArgs.Empty);
        }

        private void ShowStatus(string text, Color color)
        {
            lblStatus.Text = text;
            lblStatus.ForeColor = color;
        }

        private static decimal ToDecimal(double value)
        {
            decimal converted;
            if (decimal.TryParse(
                value.ToString(System.Globalization.CultureInfo.InvariantCulture),
                System.Globalization.NumberStyles.Float,
                System.Globalization.CultureInfo.InvariantCulture,
                out converted))
            {
                return converted;
            }
            return 0.0m;
        }

        private static double ToDouble(decimal value)
        {
            return Convert.ToDouble(value);
        }
    }
}
