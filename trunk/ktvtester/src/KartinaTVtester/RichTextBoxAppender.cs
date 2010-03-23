using System.Collections;
using System.IO;
using System.Windows.Forms;
using log4net;
using log4net.Appender;
using log4net.Repository.Hierarchy;
using System.Drawing;
using log4net.Core;

namespace KartinaTVtester {
    /// <summary>
    /// Implements a log4net appender to send output to a TextBox control.
    /// </summary>
    class RichTextBoxAppender : AppenderSkeleton {
        protected static readonly int LINES_LIMIT = 200;
        protected static readonly int LINES_TO_DELETE = 1;

        protected RichTextBox reachTextBox;

        protected Color colorDebugFg = Color.Gray;
        protected Color colorDebugBg = SystemColors.Window;
        protected Color colorInfoFg  = SystemColors.WindowText;
        protected Color colorInfoBg  = SystemColors.Control;
        protected Color colorWarnFg  = SystemColors.WindowText;
        protected Color colorWarnBg  = Color.Yellow;
        protected Color colorErrorFg = Color.White;
        protected Color colorErrorBg = Color.DarkRed;
        protected Color colorFatalFg = Color.White;
        protected Color colorFatalBg = Color.Red;


        public RichTextBox RichTextBox {
            get { return reachTextBox; }
            set { reachTextBox = value; }
        }

        public string ColorDebugFg {
            get { return colorDebugFg.ToString(); }
            set { colorDebugFg = Color.FromName(value); }
        }
        public string ColorDebugBg {
            get { return colorDebugBg.ToString(); }
            set { colorDebugBg = Color.FromName(value); }
        }
        public string ColorInfoFg {
            get { return colorInfoFg.ToString(); }
            set { colorInfoFg = Color.FromName(value); }
        }
        public string ColorInfoBg {
            get { return colorInfoBg.ToString(); }
            set { colorInfoBg = Color.FromName(value); }
        }
        public string ColorWarnFg {
            get { return colorWarnFg.ToString(); }
            set { colorWarnFg = Color.FromName(value); }
        }
        public string ColorWarnBg {
            get { return colorWarnBg.ToString(); }
            set { colorWarnBg = Color.FromName(value); }
        }
        public string ColorErrorFg {
            get { return colorErrorFg.ToString(); }
            set { colorErrorFg = Color.FromName(value); }
        }
        public string ColorErrorBg {
            get { return colorErrorBg.ToString(); }
            set { colorErrorBg = Color.FromName(value); }
        }
        public string ColorFatalFg {
            get { return colorFatalFg.ToString(); }
            set { colorFatalFg = Color.FromName(value); }
        }
        public string ColorFatalBg {
            get { return colorFatalBg.ToString(); }
            set { colorFatalBg = Color.FromName(value); }
        }


        protected override void Append(log4net.Core.LoggingEvent loggingEvent) {
            StringWriter stringWriter = new StringWriter();
            Layout.Format(stringWriter, loggingEvent);
            if (null == reachTextBox) {
                return;
            }

            // set proper global colors in RichTextBox 
            if (reachTextBox.ForeColor != colorInfoFg) {
                reachTextBox.ForeColor = colorInfoFg;
            }
            if (reachTextBox.BackColor != colorInfoBg) {
                reachTextBox.BackColor = colorInfoBg;
            }

            // set colors for currently added line
            Color fgColor = colorInfoFg;
            Color bgColor = colorInfoBg;

            if (loggingEvent.Level < Level.Info) {
                fgColor = colorDebugFg;
                bgColor = colorDebugBg;
            }
            else if (loggingEvent.Level.Equals(Level.Info)) {
                fgColor = colorInfoFg;
                bgColor = colorInfoBg;
            }
            else if (loggingEvent.Level.Equals(Level.Warn)) {
                fgColor = colorWarnFg;
                bgColor = colorWarnBg;
            }
            else if (loggingEvent.Level.Equals(Level.Error)) {
                fgColor = colorErrorFg;
                bgColor = colorErrorBg;
            }
            else if (loggingEvent.Level > Level.Error) {
                fgColor = colorFatalFg;
                bgColor = colorFatalBg;
            }

            removeObsoleteLines();
            reachTextBox.SelectionColor = fgColor;
            reachTextBox.SelectionBackColor = bgColor;
            reachTextBox.AppendText(stringWriter.ToString());
            reachTextBox.ScrollToCaret();
        }

        private void removeObsoleteLines() {
            if (reachTextBox.Lines.Length < LINES_LIMIT) {
                return;
            }

            int offset = 0;
            for (int line = 0; line < LINES_TO_DELETE; line++) {
                offset += reachTextBox.Lines[line].Length + 1;
            }

            bool isReadOnly = reachTextBox.ReadOnly;
            reachTextBox.ReadOnly = false;
            reachTextBox.Select(0, offset);
            reachTextBox.SelectedText = "";
            reachTextBox.ReadOnly = isReadOnly;
            reachTextBox.SelectionStart = reachTextBox.Text.Length;
        }
    }
}
