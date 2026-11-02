using System;
using System.Collections.Generic;
using System.Globalization;

namespace UI
{
    /// <summary>
    /// 参数化轮胎截面的全部初始数据。坐标按建模手册图 1-2 至图 1-5 录入。
    /// </summary>
    public sealed class TireSectionParameterSet
    {
        public const int ExpectedPointCount = 30;
        public const int ExpectedSegmentCount = 19;
        public const double ReferenceCrownHeight = 635.0;
        public const double ReferenceHalfWidth = 227.5;

        public double CrownHeight { get; set; }
        public double HalfWidth { get; set; }
        public double BeadCenterX { get; set; }
        public double BeadCenterY { get; set; }
        public double BeadRadius { get; set; }

        public double CrownDx { get; set; }
        public double CrownDy { get; set; }
        public double ShoulderDx { get; set; }
        public double ShoulderDy { get; set; }
        public double SidewallDx { get; set; }
        public double SidewallDy { get; set; }
        public double BeadDx { get; set; }
        public double BeadDy { get; set; }

        public double TreadThickness { get; set; }
        public double BufferThickness { get; set; }
        public double CarcassThickness { get; set; }
        public double InnerLinerThickness { get; set; }
        public double SidewallThickness { get; set; }
        public double BeadHeelThickness { get; set; }

        public bool BuildSectionFace { get; set; }
        public bool MirrorSection { get; set; }

        public List<TireSectionPoint> Points { get; private set; }
        public List<TireSectionSegment> Segments { get; private set; }

        private TireSectionParameterSet()
        {
            Points = new List<TireSectionPoint>();
            Segments = new List<TireSectionSegment>();
        }

        public static TireSectionParameterSet CreateDefault()
        {
            TireSectionParameterSet value = new TireSectionParameterSet();

            value.CrownHeight = ReferenceCrownHeight;
            value.HalfWidth = ReferenceHalfWidth;
            value.BeadCenterX = 133.3;
            value.BeadCenterY = 301.0;
            value.BeadRadius = 16.7;

            value.TreadThickness = 10.0;
            value.BufferThickness = 19.0;
            value.CarcassThickness = 7.0;
            value.InnerLinerThickness = 2.0;
            value.SidewallThickness = 2.5;
            value.BeadHeelThickness = 3.0;

            value.BuildSectionFace = true;
            value.MirrorSection = true;

            // 点索引与下方线段配方一一对应。
            value.Points.Add(new TireSectionPoint(0, "胎冠中心", 0.0, 635.0, true));
            value.Points.Add(new TireSectionPoint(1, "胎冠平台", 140.0, 635.0, false));
            value.Points.Add(new TireSectionPoint(2, "胎肩内侧", 140.0, 629.5, false));
            value.Points.Add(new TireSectionPoint(3, "胎肩圆角", 141.7, 623.8, false));
            value.Points.Add(new TireSectionPoint(4, "胎肩过渡", 146.7, 620.0, false));
            value.Points.Add(new TireSectionPoint(5, "胎肩外缘", 171.0, 610.4, false));
            value.Points.Add(new TireSectionPoint(6, "肩侧过渡", 193.5, 599.3, false));
            value.Points.Add(new TireSectionPoint(7, "胎侧上部", 220.2, 529.7, false));
            value.Points.Add(new TireSectionPoint(8, "最大宽度", 227.5, 457.0, false));
            value.Points.Add(new TireSectionPoint(9, "胎侧外弧", 211.0, 395.8, false));
            value.Points.Add(new TireSectionPoint(10, "胎侧下部", 166.0, 338.0, false));
            value.Points.Add(new TireSectionPoint(11, "胎圈肩部", 163.8, 334.8, false));
            value.Points.Add(new TireSectionPoint(12, "胎圈肩部2", 162.0, 331.3, false));
            value.Points.Add(new TireSectionPoint(13, "胎踵外侧上", 159.7, 317.4, false));
            value.Points.Add(new TireSectionPoint(14, "胎踵外侧", 159.3, 299.0, false));
            value.Points.Add(new TireSectionPoint(15, "胎踵右下", 159.3, 279.0, false));
            value.Points.Add(new TireSectionPoint(16, "胎踵左下", 109.3, 274.0, false));
            value.Points.Add(new TireSectionPoint(17, "胎踵内侧", 109.3, 290.0, false));
            value.Points.Add(new TireSectionPoint(18, "内侧胎圈肩", 112.5, 314.0, false));
            value.Points.Add(new TireSectionPoint(19, "内侧胎侧下", 122.6, 327.0, false));
            value.Points.Add(new TireSectionPoint(20, "内侧胎侧中", 130.0, 333.5, false));
            value.Points.Add(new TireSectionPoint(21, "内侧胎侧上", 140.0, 343.0, false));
            value.Points.Add(new TireSectionPoint(22, "内侧圆弧", 187.7, 398.7, false));
            value.Points.Add(new TireSectionPoint(23, "内轮廓最大宽度", 207.5, 457.0, false));
            value.Points.Add(new TireSectionPoint(24, "内轮廓胎肩下", 202.0, 506.0, false));
            value.Points.Add(new TireSectionPoint(25, "内轮廓胎肩上", 185.0, 547.2, false));
            value.Points.Add(new TireSectionPoint(26, "内轮廓肩弧", 175.7, 558.0, false));
            value.Points.Add(new TireSectionPoint(27, "内轮廓胎冠下", 164.0, 566.0, false));
            value.Points.Add(new TireSectionPoint(28, "内轮廓胎冠", 90.7, 589.0, false));
            value.Points.Add(new TireSectionPoint(29, "内轮廓中心", 0.0, 597.0, true));

            // 0 = 直线，1 = 三点圆弧。
            value.Segments.Add(TireSectionSegment.Line(0, 1));
            value.Segments.Add(TireSectionSegment.Line(1, 2));
            value.Segments.Add(TireSectionSegment.Arc(2, 3, 4));
            value.Segments.Add(TireSectionSegment.Arc(4, 5, 6));
            value.Segments.Add(TireSectionSegment.Arc(6, 7, 8));
            value.Segments.Add(TireSectionSegment.Arc(8, 9, 10));
            value.Segments.Add(TireSectionSegment.Arc(10, 11, 12));
            value.Segments.Add(TireSectionSegment.Line(12, 13));
            value.Segments.Add(TireSectionSegment.Line(13, 14));
            value.Segments.Add(TireSectionSegment.Line(14, 15));
            value.Segments.Add(TireSectionSegment.Line(15, 16));
            value.Segments.Add(TireSectionSegment.Line(16, 17));
            value.Segments.Add(TireSectionSegment.Arc(17, 18, 19));
            value.Segments.Add(TireSectionSegment.Arc(19, 20, 21));
            value.Segments.Add(TireSectionSegment.Arc(21, 22, 23));
            value.Segments.Add(TireSectionSegment.Arc(23, 24, 25));
            value.Segments.Add(TireSectionSegment.Arc(25, 26, 27));
            value.Segments.Add(TireSectionSegment.Arc(27, 28, 29));
            value.Segments.Add(TireSectionSegment.Line(29, 0));

            return value;
        }

        public TireSectionParameterSet Clone()
        {
            TireSectionParameterSet copy = CreateDefault();
            copy.CopyValuesFrom(this);
            return copy;
        }

        public void CopyValuesFrom(TireSectionParameterSet source)
        {
            if (source == null)
                return;

            CrownHeight = source.CrownHeight;
            HalfWidth = source.HalfWidth;
            BeadCenterX = source.BeadCenterX;
            BeadCenterY = source.BeadCenterY;
            BeadRadius = source.BeadRadius;

            CrownDx = source.CrownDx;
            CrownDy = source.CrownDy;
            ShoulderDx = source.ShoulderDx;
            ShoulderDy = source.ShoulderDy;
            SidewallDx = source.SidewallDx;
            SidewallDy = source.SidewallDy;
            BeadDx = source.BeadDx;
            BeadDy = source.BeadDy;

            TreadThickness = source.TreadThickness;
            BufferThickness = source.BufferThickness;
            CarcassThickness = source.CarcassThickness;
            InnerLinerThickness = source.InnerLinerThickness;
            SidewallThickness = source.SidewallThickness;
            BeadHeelThickness = source.BeadHeelThickness;

            BuildSectionFace = source.BuildSectionFace;
            MirrorSection = source.MirrorSection;

            Points.Clear();
            foreach (TireSectionPoint point in source.Points)
                Points.Add(point.Clone());

            Segments.Clear();
            foreach (TireSectionSegment segment in source.Segments)
                Segments.Add(segment.Clone());
        }

        public double[] ToPointTable()
        {
            List<double> values = new List<double>(Points.Count * 5);
            foreach (TireSectionPoint point in Points)
            {
                values.Add(point.BaseX);
                values.Add(point.BaseY);
                values.Add(point.DeltaX);
                values.Add(point.DeltaY);
                values.Add(point.Locked ? 1.0 : 0.0);
            }
            return values.ToArray();
        }

        public int[] ToSegmentTable()
        {
            List<int> values = new List<int>(Segments.Count * 4);
            foreach (TireSectionSegment segment in Segments)
            {
                values.Add(segment.Kind);
                values.Add(segment.StartIndex);
                values.Add(segment.MiddleIndex);
                values.Add(segment.EndIndex);
            }
            return values.ToArray();
        }

        public double[] ToThicknessTable()
        {
            return new double[]
            {
                TreadThickness,
                BufferThickness,
                CarcassThickness,
                InnerLinerThickness,
                SidewallThickness,
                BeadHeelThickness,
                BeadCenterX,
                BeadCenterY,
                BeadRadius
            };
        }

        public double[] ToAdjustmentTable()
        {
            return new double[]
            {
                CrownDx,
                CrownDy,
                ShoulderDx,
                ShoulderDy,
                SidewallDx,
                SidewallDy,
                BeadDx,
                BeadDy,
                CrownHeight,
                HalfWidth
            };
        }

        public string Validate()
        {
            if (Points.Count != ExpectedPointCount)
                return string.Format(CultureInfo.CurrentCulture,
                    "关键点数量必须为 {0} 个。", ExpectedPointCount);

            if (Segments.Count != ExpectedSegmentCount)
                return string.Format(CultureInfo.CurrentCulture,
                    "线段配方数量必须为 {0} 段。", ExpectedSegmentCount);

            if (CrownHeight <= 0.0 || HalfWidth <= 0.0)
                return "轮胎高度和半宽必须大于 0。";

            if (BeadRadius <= 0.05)
                return "钢丝圈半径必须大于 0.05 mm。";

            double[] thicknessValues = ToThicknessTable();
            for (int i = 0; i < 6; i++)
            {
                if (double.IsNaN(thicknessValues[i])
                    || double.IsInfinity(thicknessValues[i])
                    || thicknessValues[i] <= 0.05)
                {
                    return "所有材料层厚度必须大于 0.05 mm。";
                }
            }

            foreach (TireSectionPoint point in Points)
            {
                if (!IsFinite(point.BaseX) || !IsFinite(point.BaseY)
                    || !IsFinite(point.DeltaX) || !IsFinite(point.DeltaY))
                {
                    return string.Format(CultureInfo.CurrentCulture,
                        "点 {0} 含有无效数值。", point.Index);
                }
            }

            foreach (TireSectionSegment segment in Segments)
            {
                if (!IsValidIndex(segment.StartIndex)
                    || !IsValidIndex(segment.EndIndex))
                {
                    return "线段配方引用了不存在的关键点。";
                }

                if (segment.Kind == 1 && !IsValidIndex(segment.MiddleIndex))
                    return "圆弧配方缺少有效的中间点。";
            }

            return string.Empty;
        }

        private bool IsValidIndex(int index)
        {
            return index >= 0 && index < Points.Count;
        }

        private static bool IsFinite(double value)
        {
            return !double.IsNaN(value) && !double.IsInfinity(value);
        }
    }

    public sealed class TireSectionPoint
    {
        public int Index { get; set; }
        public string Zone { get; set; }
        public double BaseX { get; set; }
        public double BaseY { get; set; }
        public double DeltaX { get; set; }
        public double DeltaY { get; set; }
        public bool Locked { get; set; }

        public TireSectionPoint(
            int index,
            string zone,
            double baseX,
            double baseY,
            bool locked)
        {
            Index = index;
            Zone = zone;
            BaseX = baseX;
            BaseY = baseY;
            Locked = locked;
        }

        public TireSectionPoint Clone()
        {
            return new TireSectionPoint(
                Index, Zone, BaseX, BaseY, Locked)
            {
                DeltaX = DeltaX,
                DeltaY = DeltaY
            };
        }
    }

    public sealed class TireSectionSegment
    {
        public int Kind { get; set; }
        public int StartIndex { get; set; }
        public int MiddleIndex { get; set; }
        public int EndIndex { get; set; }

        public static TireSectionSegment Line(int start, int end)
        {
            return new TireSectionSegment
            {
                Kind = 0,
                StartIndex = start,
                MiddleIndex = -1,
                EndIndex = end
            };
        }

        public static TireSectionSegment Arc(int start, int middle, int end)
        {
            return new TireSectionSegment
            {
                Kind = 1,
                StartIndex = start,
                MiddleIndex = middle,
                EndIndex = end
            };
        }

        public TireSectionSegment Clone()
        {
            return new TireSectionSegment
            {
                Kind = Kind,
                StartIndex = StartIndex,
                MiddleIndex = MiddleIndex,
                EndIndex = EndIndex
            };
        }
    }

    public sealed class ParametricTireRequestEventArgs : EventArgs
    {
        public TireSectionParameterSet Parameters { get; private set; }
        public bool Success { get; set; }
        public string Message { get; set; }

        public ParametricTireRequestEventArgs(
            TireSectionParameterSet parameters)
        {
            Parameters = parameters;
            Success = false;
            Message = string.Empty;
        }
    }
}
