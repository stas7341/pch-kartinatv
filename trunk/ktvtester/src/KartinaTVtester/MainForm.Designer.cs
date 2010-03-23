namespace KartinaTVtester {
    partial class MainForm {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.tabsLogger = new System.Windows.Forms.TabControl();
            this.tabHelper = new System.Windows.Forms.TabPage();
            this.textBoxHelper = new System.Windows.Forms.RichTextBox();
            this.buttonAuthorize = new System.Windows.Forms.Button();
            this.buttonChannels = new System.Windows.Forms.Button();
            this.labelCookie = new System.Windows.Forms.Label();
            this.textBoxCookie = new System.Windows.Forms.TextBox();
            this.labelUser = new System.Windows.Forms.Label();
            this.textBoxUser = new System.Windows.Forms.TextBox();
            this.textBoxPass = new System.Windows.Forms.TextBox();
            this.labelPass = new System.Windows.Forms.Label();
            this.buttonStream = new System.Windows.Forms.Button();
            this.buttonEpg = new System.Windows.Forms.Button();
            this.buttonTimeshift = new System.Windows.Forms.Button();
            this.buttonServer = new System.Windows.Forms.Button();
            this.labelChannelId = new System.Windows.Forms.Label();
            this.textBoxStreamUrl = new System.Windows.Forms.TextBox();
            this.labelStreamUrl = new System.Windows.Forms.Label();
            this.comboBoxChannelId = new System.Windows.Forms.ComboBox();
            this.buttonPlay = new System.Windows.Forms.Button();
            this.tabsLogger.SuspendLayout();
            this.tabHelper.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabsLogger
            // 
            this.tabsLogger.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this.tabsLogger.Anchor = ((System.Windows.Forms.AnchorStyles) ((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tabsLogger.Controls.Add(this.tabHelper);
            this.tabsLogger.Location = new System.Drawing.Point(12, 58);
            this.tabsLogger.Name = "tabsLogger";
            this.tabsLogger.SelectedIndex = 0;
            this.tabsLogger.Size = new System.Drawing.Size(501, 332);
            this.tabsLogger.TabIndex = 0;
            this.tabsLogger.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.tabsLogger_MouseDoubleClick);
            this.tabsLogger.MouseClick += new System.Windows.Forms.MouseEventHandler(this.tabsLogger_MouseClick);
            // 
            // tabHelper
            // 
            this.tabHelper.Controls.Add(this.textBoxHelper);
            this.tabHelper.Location = new System.Drawing.Point(4, 4);
            this.tabHelper.Name = "tabHelper";
            this.tabHelper.Padding = new System.Windows.Forms.Padding(3);
            this.tabHelper.Size = new System.Drawing.Size(493, 306);
            this.tabHelper.TabIndex = 0;
            this.tabHelper.Text = "Help";
            this.tabHelper.UseVisualStyleBackColor = true;
            // 
            // textBoxHelper
            // 
            this.textBoxHelper.Anchor = ((System.Windows.Forms.AnchorStyles) ((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxHelper.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte) (0)));
            this.textBoxHelper.Location = new System.Drawing.Point(0, 0);
            this.textBoxHelper.Name = "textBoxHelper";
            this.textBoxHelper.ReadOnly = true;
            this.textBoxHelper.Size = new System.Drawing.Size(493, 303);
            this.textBoxHelper.TabIndex = 1;
            this.textBoxHelper.Text = "";
            // 
            // buttonAuthorize
            // 
            this.buttonAuthorize.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonAuthorize.Location = new System.Drawing.Point(519, 110);
            this.buttonAuthorize.Name = "buttonAuthorize";
            this.buttonAuthorize.Size = new System.Drawing.Size(175, 23);
            this.buttonAuthorize.TabIndex = 1;
            this.buttonAuthorize.Text = "1. Authorize";
            this.buttonAuthorize.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonAuthorize.UseVisualStyleBackColor = true;
            this.buttonAuthorize.Click += new System.EventHandler(this.buttonAuthorize_Click);
            // 
            // buttonChannels
            // 
            this.buttonChannels.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonChannels.Location = new System.Drawing.Point(519, 139);
            this.buttonChannels.Name = "buttonChannels";
            this.buttonChannels.Size = new System.Drawing.Size(175, 23);
            this.buttonChannels.TabIndex = 2;
            this.buttonChannels.Text = "2. Get channels list";
            this.buttonChannels.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonChannels.UseVisualStyleBackColor = true;
            this.buttonChannels.Click += new System.EventHandler(this.buttonChannels_Click);
            // 
            // labelCookie
            // 
            this.labelCookie.AutoSize = true;
            this.labelCookie.Location = new System.Drawing.Point(12, 9);
            this.labelCookie.Name = "labelCookie";
            this.labelCookie.Size = new System.Drawing.Size(43, 13);
            this.labelCookie.TabIndex = 3;
            this.labelCookie.Text = "Cookie:";
            // 
            // textBoxCookie
            // 
            this.textBoxCookie.Anchor = ((System.Windows.Forms.AnchorStyles) (((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxCookie.Location = new System.Drawing.Point(86, 6);
            this.textBoxCookie.Name = "textBoxCookie";
            this.textBoxCookie.ReadOnly = true;
            this.textBoxCookie.Size = new System.Drawing.Size(608, 20);
            this.textBoxCookie.TabIndex = 4;
            this.textBoxCookie.Text = "No cookie set";
            // 
            // labelUser
            // 
            this.labelUser.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.labelUser.AutoSize = true;
            this.labelUser.Location = new System.Drawing.Point(515, 61);
            this.labelUser.Name = "labelUser";
            this.labelUser.Size = new System.Drawing.Size(32, 13);
            this.labelUser.TabIndex = 5;
            this.labelUser.Text = "User:";
            // 
            // textBoxUser
            // 
            this.textBoxUser.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxUser.Location = new System.Drawing.Point(584, 58);
            this.textBoxUser.Name = "textBoxUser";
            this.textBoxUser.Size = new System.Drawing.Size(110, 20);
            this.textBoxUser.TabIndex = 6;
            this.textBoxUser.Text = "148";
            this.textBoxUser.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.authorize_KeyPress);
            // 
            // textBoxPass
            // 
            this.textBoxPass.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxPass.Location = new System.Drawing.Point(584, 84);
            this.textBoxPass.Name = "textBoxPass";
            this.textBoxPass.Size = new System.Drawing.Size(110, 20);
            this.textBoxPass.TabIndex = 8;
            this.textBoxPass.Text = "841";
            this.textBoxPass.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.authorize_KeyPress);
            // 
            // labelPass
            // 
            this.labelPass.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.labelPass.AutoSize = true;
            this.labelPass.Location = new System.Drawing.Point(515, 87);
            this.labelPass.Name = "labelPass";
            this.labelPass.Size = new System.Drawing.Size(33, 13);
            this.labelPass.TabIndex = 7;
            this.labelPass.Text = "Pass:";
            // 
            // buttonStream
            // 
            this.buttonStream.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonStream.Location = new System.Drawing.Point(519, 206);
            this.buttonStream.Name = "buttonStream";
            this.buttonStream.Size = new System.Drawing.Size(175, 23);
            this.buttonStream.TabIndex = 9;
            this.buttonStream.Text = "3. Get stream URL of channel";
            this.buttonStream.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonStream.UseVisualStyleBackColor = true;
            this.buttonStream.Click += new System.EventHandler(this.buttonStream_Click);
            // 
            // buttonEpg
            // 
            this.buttonEpg.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonEpg.Location = new System.Drawing.Point(519, 235);
            this.buttonEpg.Name = "buttonEpg";
            this.buttonEpg.Size = new System.Drawing.Size(175, 23);
            this.buttonEpg.TabIndex = 10;
            this.buttonEpg.Text = "4. Get EPG of channel";
            this.buttonEpg.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonEpg.UseVisualStyleBackColor = true;
            this.buttonEpg.Click += new System.EventHandler(this.buttonEpg_Click);
            // 
            // buttonTimeshift
            // 
            this.buttonTimeshift.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonTimeshift.Location = new System.Drawing.Point(519, 316);
            this.buttonTimeshift.Name = "buttonTimeshift";
            this.buttonTimeshift.Size = new System.Drawing.Size(175, 23);
            this.buttonTimeshift.TabIndex = 11;
            this.buttonTimeshift.Text = "5. Get time shift";
            this.buttonTimeshift.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonTimeshift.UseVisualStyleBackColor = true;
            this.buttonTimeshift.Click += new System.EventHandler(this.buttonTimeshift_Click);
            // 
            // buttonServer
            // 
            this.buttonServer.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonServer.Location = new System.Drawing.Point(519, 345);
            this.buttonServer.Name = "buttonServer";
            this.buttonServer.Size = new System.Drawing.Size(175, 23);
            this.buttonServer.TabIndex = 12;
            this.buttonServer.Text = "6. Get broadcasting server";
            this.buttonServer.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonServer.UseVisualStyleBackColor = true;
            this.buttonServer.Click += new System.EventHandler(this.buttonServer_Click);
            // 
            // labelChannelId
            // 
            this.labelChannelId.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.labelChannelId.AutoSize = true;
            this.labelChannelId.Location = new System.Drawing.Point(515, 183);
            this.labelChannelId.Name = "labelChannelId";
            this.labelChannelId.Size = new System.Drawing.Size(63, 13);
            this.labelChannelId.TabIndex = 13;
            this.labelChannelId.Text = "Channel ID:";
            // 
            // textBoxStreamUrl
            // 
            this.textBoxStreamUrl.Anchor = ((System.Windows.Forms.AnchorStyles) (((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxStreamUrl.Location = new System.Drawing.Point(86, 32);
            this.textBoxStreamUrl.Name = "textBoxStreamUrl";
            this.textBoxStreamUrl.ReadOnly = true;
            this.textBoxStreamUrl.Size = new System.Drawing.Size(542, 20);
            this.textBoxStreamUrl.TabIndex = 16;
            this.textBoxStreamUrl.Text = "No stream";
            // 
            // labelStreamUrl
            // 
            this.labelStreamUrl.AutoSize = true;
            this.labelStreamUrl.Location = new System.Drawing.Point(12, 35);
            this.labelStreamUrl.Name = "labelStreamUrl";
            this.labelStreamUrl.Size = new System.Drawing.Size(68, 13);
            this.labelStreamUrl.TabIndex = 15;
            this.labelStreamUrl.Text = "Stream URL:";
            // 
            // comboBoxChannelId
            // 
            this.comboBoxChannelId.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxChannelId.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
            this.comboBoxChannelId.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.comboBoxChannelId.FormattingEnabled = true;
            this.comboBoxChannelId.Location = new System.Drawing.Point(584, 179);
            this.comboBoxChannelId.Name = "comboBoxChannelId";
            this.comboBoxChannelId.Size = new System.Drawing.Size(110, 21);
            this.comboBoxChannelId.TabIndex = 17;
            this.comboBoxChannelId.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.getStream_KeyPress);
            // 
            // buttonPlay
            // 
            this.buttonPlay.Anchor = ((System.Windows.Forms.AnchorStyles) ((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonPlay.Location = new System.Drawing.Point(634, 30);
            this.buttonPlay.Name = "buttonPlay";
            this.buttonPlay.Size = new System.Drawing.Size(60, 23);
            this.buttonPlay.TabIndex = 18;
            this.buttonPlay.Text = "Play";
            this.buttonPlay.UseVisualStyleBackColor = true;
            this.buttonPlay.Click += new System.EventHandler(this.buttonPlay_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(706, 402);
            this.Controls.Add(this.buttonPlay);
            this.Controls.Add(this.comboBoxChannelId);
            this.Controls.Add(this.textBoxStreamUrl);
            this.Controls.Add(this.labelStreamUrl);
            this.Controls.Add(this.labelChannelId);
            this.Controls.Add(this.buttonServer);
            this.Controls.Add(this.buttonTimeshift);
            this.Controls.Add(this.buttonEpg);
            this.Controls.Add(this.buttonStream);
            this.Controls.Add(this.textBoxPass);
            this.Controls.Add(this.labelPass);
            this.Controls.Add(this.textBoxUser);
            this.Controls.Add(this.labelUser);
            this.Controls.Add(this.textBoxCookie);
            this.Controls.Add(this.labelCookie);
            this.Controls.Add(this.buttonChannels);
            this.Controls.Add(this.buttonAuthorize);
            this.Controls.Add(this.tabsLogger);
            this.Icon = ((System.Drawing.Icon) (resources.GetObject("$this.Icon")));
            this.Name = "MainForm";
            this.Text = "KartinaTV Demo";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.tabsLogger.ResumeLayout(false);
            this.tabHelper.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl tabsLogger;
        private System.Windows.Forms.Button buttonAuthorize;
        private System.Windows.Forms.Button buttonChannels;
        private System.Windows.Forms.TabPage tabHelper;
        private System.Windows.Forms.Label labelCookie;
        private System.Windows.Forms.TextBox textBoxCookie;
        private System.Windows.Forms.Label labelUser;
        private System.Windows.Forms.TextBox textBoxUser;
        private System.Windows.Forms.TextBox textBoxPass;
        private System.Windows.Forms.Label labelPass;
        private System.Windows.Forms.Button buttonStream;
        private System.Windows.Forms.Button buttonEpg;
        private System.Windows.Forms.Button buttonTimeshift;
        private System.Windows.Forms.Button buttonServer;
        private System.Windows.Forms.RichTextBox textBoxHelper;
        private System.Windows.Forms.Label labelChannelId;
        private System.Windows.Forms.TextBox textBoxStreamUrl;
        private System.Windows.Forms.Label labelStreamUrl;
        private System.Windows.Forms.ComboBox comboBoxChannelId;
        private System.Windows.Forms.Button buttonPlay;
    }
}

