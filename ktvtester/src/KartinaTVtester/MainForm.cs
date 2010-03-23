using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Xml;
using System.Windows.Forms;
using log4net;
using log4net.Appender;
using log4net.Config;

using System.Security.Permissions;
using Microsoft.Win32;

namespace KartinaTVtester {
    public partial class MainForm : Form {

        private static readonly ILog logger =
               LogManager.GetLogger(typeof(MainForm));

        protected int tabCounter = 0;
        protected KtvFunctions ktvFunctions = new KtvFunctions();

        public MainForm() {
            InitializeComponent();
        }

        protected void MainForm_Load(object sender, EventArgs e) {
            activateLogger(textBoxHelper);
            logger.Info("Hello from KartinaTVtester!");
            textBoxUser.Text = Properties.Settings.Default.User;
            textBoxPass.Text = Properties.Settings.Default.Pass;
            updateCookieText();
        }

        protected void updateCookieText() {
            textBoxCookie.Text = ktvFunctions.Cookie;
        }

        protected bool activateLogger(RichTextBox textBox) {
            IAppender[] iAppenderArray =
                logger.Logger.Repository.GetAppenders();
            foreach (IAppender appender in iAppenderArray) {
                if (appender is RichTextBoxAppender &&
                    "RichTextBoxAppender".Equals(appender.Name)) {
                    ((RichTextBoxAppender) appender).RichTextBox = textBox;
                    return true;
                }
            }
            return false;
        }

        protected void createNewLogTab(string name) {
            TabPage newTab = new TabPage(++tabCounter + ". " + name);
            RichTextBox newText = new RichTextBox();

            newTab.Controls.Add(newText);
            newTab.Location = this.tabHelper.Location;
            newTab.Padding = this.tabHelper.Padding;
            newTab.Size = this.tabHelper.Size;
            newTab.UseVisualStyleBackColor =
                this.tabHelper.UseVisualStyleBackColor;

            newText.Anchor = this.textBoxHelper.Anchor;
            newText.Font = this.textBoxHelper.Font;
            newText.Location = this.textBoxHelper.Location;
            newText.Multiline = this.textBoxHelper.Multiline;
            newText.Size = this.textBoxHelper.Size;
            newText.ReadOnly = this.textBoxHelper.ReadOnly;
            newText.WordWrap = this.textBoxHelper.WordWrap;

            this.tabsLogger.TabPages.Add(newTab);
            this.tabsLogger.SelectedTab = newTab;

            activateLogger(newText);
            logger.Info("New tab created: " + newTab.Text);
        }

        private void tabsLogger_MouseClick(object sender, MouseEventArgs e) {
            if (e.Button != MouseButtons.Middle) {
                return;
            }
            TabControl tabs = (TabControl) sender;
            for (int i = tabs.TabPages.Count - 1; i > 0; i--) {
                if (tabs.GetTabRect(i).Contains(e.Location)) {
                    // correct selection if needed
                    // and reactivate initial logger
                    if (tabs.SelectedIndex == i) {
                        tabs.SelectedIndex = i - 1;
                        activateLogger(textBoxHelper);
                    }
                    tabs.TabPages.RemoveAt(i);
                    break;
                }
            }
        }

        private void tabsLogger_MouseDoubleClick(object sender, MouseEventArgs e) {
            if (e.Button != MouseButtons.Left) {
                return;
            }
            TabControl tabs = (TabControl) sender;
            for (int i = tabs.TabPages.Count - 1; i > 0; i--) {
                if (tabs.GetTabRect(i).Contains(e.Location)) {
                    RichTextBox text = (RichTextBox) tabs.TabPages[i].Controls[0];
                    text.WordWrap = !text.WordWrap;
                    break;
                }
            }
        }

        protected void authorize_KeyPress(object sender, KeyPressEventArgs e) {
            if (e.KeyChar == (char) Keys.Enter) {
                buttonAuthorize_Click(textBoxUser, null);
            }
        }

        protected void buttonAuthorize_Click(object sender, EventArgs e) {
            createNewLogTab("Authorize");
            ktvFunctions.setCredentials(textBoxUser.Text, textBoxPass.Text);
            ktvFunctions.authorize();
            updateCookieText();
        }

        protected void buttonChannels_Click(object sender, EventArgs e) {
            createNewLogTab("Channels");
            String html = ktvFunctions.getChannelsList();
            updateCookieText();
            comboBoxChannelId.Items.Clear();

            logger.Info("Parsing channels list");
            XmlDocument doc = new XmlDocument();
            doc.InnerXml = html;
            XmlElement root = doc.DocumentElement;
            logger.Debug("Update time: " + root.GetAttribute("clienttime"));
            foreach (XmlNode category in root.ChildNodes) {
                if (category.NodeType != XmlNodeType.Element) {
                    continue;
                }

                string catId = category.Attributes["id"].Value;
                string catTitle = category.Attributes["title"].Value;
                string catColor = category.Attributes["color"].Value;
                logger.Debug(String.Format("Group: ID={0}, Title={1}, Color={2}",
                    catId, catTitle, catColor));

                comboBoxChannelId.Items.Add(
                    "--- " + catTitle + " --------------------------------");

                foreach (XmlNode channel in category.ChildNodes) {
                    if (channel.NodeType != XmlNodeType.Element) {
                        continue;
                    }
                    string id = channel.Attributes["id"].Value;
                    string title = channel.Attributes["title"].Value;

                    logger.Debug(String.Format("Channel: ID={0}, Title={1}",
                        id, title));

                    comboBoxChannelId.Items.Add(String.Format(
                        "{0} ({1})", title, id));

                    if (comboBoxChannelId.SelectedIndex < 1) {
                        comboBoxChannelId.SelectedIndex =
                            comboBoxChannelId.Items.Count - 1;
                    }
                }
            }
        }

        protected int getSelectedChannelId() {
            string id = Regex.Replace(comboBoxChannelId.Text,
                ".*\\((\\d+)\\)", "$1");
            try {
                return Int32.Parse(id);
            }
            catch (FormatException) {
                logger.Error("Wrong channel ID: " + id);
            }
            return -1;
        }

        protected void getStream_KeyPress(object sender, KeyPressEventArgs e) {
            if (e.KeyChar == (char) Keys.Enter) {
                buttonStream_Click(buttonStream, null);
            }
        }


        protected void buttonStream_Click(object sender, EventArgs e) {
            int channelId = getSelectedChannelId();
            if (-1 == channelId) {
                logger.Error("No channel selected, obtain channel list first");
                return;
            }
            createNewLogTab("Stream");
            string html = ktvFunctions.getStreamUrl(channelId);
            updateCookieText();
            logger.Info("Parsing stream URL");
            string url = Regex.Replace(html,
                ".*url=\"http(/ts|)([^ \"]*).*", "http$2");
            if (!url.StartsWith("http://")) {
                url = "No URL found";
            }
            textBoxStreamUrl.Text = url;
        }

        protected void buttonEpg_Click(object sender, EventArgs e) {
            int channelId = getSelectedChannelId();
            if (-1 == channelId) {
                logger.Error("No channel selected, obtain channel list first");
                return;
            }
            createNewLogTab("EPG");
            ktvFunctions.getEpg(channelId);
            updateCookieText();
        }

        protected void buttonTimeshift_Click(object sender, EventArgs e) {
            createNewLogTab("TimeShift");
            ktvFunctions.getTimeShift();
            updateCookieText();
        }

        protected void buttonServer_Click(object sender, EventArgs e) {
            createNewLogTab("Server");
            ktvFunctions.getBroadcastingServer();
            updateCookieText();
        }

        protected void buttonPlay_Click(object sender, EventArgs e) {
            String url = textBoxStreamUrl.Text;
            if (!url.StartsWith("http://")) {
                logger.Error("Stream URL is invalid");
                return;
            }

            string[] playerPathes = new string[] {
                @"Applications\vlc.exe\shell\open\command",
                @"Applications\vlc.exe\shell\Play\command"
            };

            string player = Properties.Settings.Default.PlayerCmd;
            if (null == player || 0 == player.Length) {
                foreach (string playerPath in playerPathes) {
                    player = getPlayer(playerPath);
                    if (null != player) {
                        break;
                    }
                }
                if (null == player) {
                    logger.Error("No suitable player found!");
                    return;
                }
            }

            try {
                logger.Info("Starting " + player);
                System.Diagnostics.Process p = new System.Diagnostics.Process();
                p.StartInfo.FileName = player;
                p.StartInfo.Arguments = url;
                p.Start();
            }
            catch (Exception exception) {
                logger.Error("Cannot start player: " + player +
                    ": " + exception.Message);
            }
        }


        protected string getPlayer(string key) {
            logger.Debug("Looking for " + key);
            RegistryKey registryKey = Registry.ClassesRoot.OpenSubKey(key, false);
            if (null != registryKey) {
                string value = (string) registryKey.GetValue(null, null);
                logger.Info(registryKey + " => " + value);
                string file = null;
                if (value.StartsWith("\"")) {
                    file = value.Split('"')[1];
                }
                else {
                    file = value.Split(' ')[0];
                }
                logger.Info("Found " + key + ": " + file);
                return file;
            }
            logger.Debug("Not found " + key);
            return null;
        }
    }
}
