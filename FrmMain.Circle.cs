using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace UI
{
    public partial class FrmMain
    {
        private ToolStripButton fixedDiameterCircleButton;

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            if (fixedDiameterCircleButton != null)
                return;

            fixedDiameterCircleButton = new ToolStripButton("直径20圆");
            fixedDiameterCircleButton.DisplayStyle =
                ToolStripItemDisplayStyle.Text;
            fixedDiameterCircleButton.ToolTipText =
                "在 XOY 平面原点创建一个直径为 20 mm 的圆";
            fixedDiameterCircleButton.Click +=
                new EventHandler(FixedDiameterCircleButton_Click);
            toolStrip1.Items.Add(fixedDiameterCircleButton);
        }

        private void FixedDiameterCircleButton_Click(
            object sender,
            EventArgs e)
        {
            const double diameter = 20.0;
            const double radius = diameter / 2.0;

            if (myOCCTProxy == null)
            {
                MessageBox.Show(this, "OCCT 代理尚未初始化。", "创建圆",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            int newId = globalShapeId;
            if (!myOCCTProxy.CreateCircle(
                newId, 0.0, 0.0, 0.0, radius))
            {
                MessageBox.Show(this,
                    "创建圆失败，请检查 OCCT 视图是否已初始化。",
                    "创建圆",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
                return;
            }

            globalShapeId++;
            AddFixedCircleToTree(newId, diameter);
            LogMessage(string.Format(
                "已在原点创建直径 {0:0.##} mm 的圆，形状 ID={1}。",
                diameter,
                newId));

            myOCCTProxy.ZoomAllView();
            myOCCTProxy.UpdateCurrentViewer();
        }

        private void AddFixedCircleToTree(
            int shapeId,
            double diameter)
        {
            if (treeView1.Nodes.Count == 0)
            {
                TreeNode root = new TreeNode("轮胎模型: 手动创建");
                treeView1.Nodes.Add(root);
            }

            TreeNode rootNode = treeView1.Nodes[0];
            Color componentColor =
                hmColorPalette[(componentCounter - 1) % hmColorPalette.Length];
            TreeNode componentNode = new TreeNode(
                string.Format("Component {0}: 直径20圆", componentCounter));
            componentNode.Tag = componentColor;
            componentNode.ForeColor = componentColor;

            TreeNode circleNode = new TreeNode(
                string.Format("圆 (直径 {0:0.##} mm)", diameter));
            circleNode.Tag = shapeId;
            componentNode.Nodes.Add(circleNode);
            rootNode.Nodes.Add(componentNode);
            layerShapesMap[componentNode] = new List<int> { shapeId };

            componentCounter++;
            currentComponentNode = componentNode;
            currentLineLayerNode = componentNode;
            rootNode.ExpandAll();
            treeView1.SelectedNode = circleNode;
        }
    }
}
