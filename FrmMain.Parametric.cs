using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace UI
{
    public partial class FrmMain
    {
        private void tsbParametricTire_Click(object sender, EventArgs e)
        {
            if (myOCCTProxy == null)
            {
                MessageBox.Show(this, "OCCT 代理尚未初始化。", "参数化建模",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            myOCCTProxy.CancelParametricTirePreview();

            using (FrmParametricTireInput form = new FrmParametricTireInput())
            {
                form.PreviewRequested += HandleParametricTirePreview;
                form.GenerateRequested += HandleParametricTireGenerate;
                form.CancelRequested += HandleParametricTireCancel;
                form.ShowDialog(this);
            }

            myOCCTProxy.CancelParametricTirePreview();
            myOCCTProxy.UpdateCurrentViewer();
        }

        private void HandleParametricTirePreview(
            object sender,
            ParametricTireRequestEventArgs args)
        {
            try
            {
                bool success = myOCCTProxy.PreviewParametricTire(
                    args.Parameters.ToPointTable(),
                    args.Parameters.ToSegmentTable(),
                    args.Parameters.ToThicknessTable(),
                    args.Parameters.ToAdjustmentTable(),
                    args.Parameters.BuildSectionFace,
                    args.Parameters.MirrorSection,
                    geometryTolerance);

                args.Success = success;
                if (success)
                {
                    myOCCTProxy.ZoomAllView();
                    myOCCTProxy.UpdateCurrentViewer();
                }
                else
                {
                    args.Message = myOCCTProxy.GetLastParametricTireError();
                }
            }
            catch (Exception exception)
            {
                args.Success = false;
                args.Message = "预览失败：" + exception.Message;
            }
        }

        private void HandleParametricTireGenerate(
            object sender,
            ParametricTireRequestEventArgs args)
        {
            try
            {
                int startingId = globalShapeId;
                int[] generatedIds = myOCCTProxy.BuildParametricTire(
                    args.Parameters.ToPointTable(),
                    args.Parameters.ToSegmentTable(),
                    args.Parameters.ToThicknessTable(),
                    args.Parameters.ToAdjustmentTable(),
                    args.Parameters.BuildSectionFace,
                    args.Parameters.MirrorSection,
                    startingId,
                    geometryTolerance);

                if (generatedIds == null || generatedIds.Length == 0)
                {
                    args.Success = false;
                    args.Message = myOCCTProxy.GetLastParametricTireError();
                    return;
                }

                globalShapeId = startingId + generatedIds.Length;
                AddParametricTireComponent(generatedIds);
                args.Success = true;
                args.Message = string.Format(
                    "参数化轮胎截面生成完成，共 {0} 个几何对象。",
                    generatedIds.Length);
                LogMessage(args.Message);

                myOCCTProxy.ZoomAllView();
                myOCCTProxy.UpdateCurrentViewer();
            }
            catch (Exception exception)
            {
                args.Success = false;
                args.Message = "生成失败：" + exception.Message;
                LogMessage(args.Message);
            }
        }

        private void HandleParametricTireCancel(object sender, EventArgs e)
        {
            if (myOCCTProxy != null)
            {
                myOCCTProxy.CancelParametricTirePreview();
                myOCCTProxy.UpdateCurrentViewer();
            }
        }

        private void AddParametricTireComponent(int[] shapeIds)
        {
            if (treeView1.Nodes.Count == 0)
            {
                TreeNode root = new TreeNode("轮胎模型: 参数化建模");
                treeView1.Nodes.Add(root);
            }

            TreeNode rootNode = treeView1.Nodes[0];
            Color componentColor =
                hmColorPalette[(componentCounter - 1) % hmColorPalette.Length];
            TreeNode componentNode = new TreeNode(
                string.Format("Component {0}: 参数化轮胎", componentCounter));
            componentNode.Tag = componentColor;
            componentNode.ForeColor = componentColor;

            TreeNode summaryNode = new TreeNode(
                string.Format("参数化截面 ({0} 个对象)", shapeIds.Length));
            componentNode.Nodes.Add(summaryNode);
            rootNode.Nodes.Add(componentNode);
            layerShapesMap[componentNode] = new List<int>(shapeIds);

            componentCounter++;
            currentComponentNode = componentNode;
            currentLineLayerNode = componentNode;
            rootNode.ExpandAll();
            treeView1.SelectedNode = componentNode;
        }
    }
}
