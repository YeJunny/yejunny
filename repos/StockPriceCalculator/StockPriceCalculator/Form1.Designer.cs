namespace StockPriceCalculator
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.CompanyName = new System.Windows.Forms.TextBox();
            this.CompanyNameButton = new System.Windows.Forms.Button();
            this.Result = new System.Windows.Forms.Label();
            this.Price80 = new System.Windows.Forms.Label();
            this.Price80Result = new System.Windows.Forms.TextBox();
            this.StockPriceUnit = new System.Windows.Forms.Label();
            this.Price90 = new System.Windows.Forms.Label();
            this.Price90Result = new System.Windows.Forms.TextBox();
            this.FairPrice = new System.Windows.Forms.Label();
            this.FairPriceResult = new System.Windows.Forms.TextBox();
            this.ImportTXTStore = new System.Windows.Forms.Button();
            this.Detail = new System.Windows.Forms.Label();
            this.ROE = new System.Windows.Forms.Label();
            this.ROEResult = new System.Windows.Forms.TextBox();
            this.CompanyValue = new System.Windows.Forms.Label();
            this.CompanyValueResult = new System.Windows.Forms.TextBox();
            this.EquityOfControllingShareholders = new System.Windows.Forms.Label();
            this.EquityOfControllingShareholdersResult = new System.Windows.Forms.TextBox();
            this.OriginalUnit = new System.Windows.Forms.Label();
            this.ExportExcel = new System.Windows.Forms.Button();
            this.NotKorea = new System.Windows.Forms.CheckBox();
            this.MarketValue = new System.Windows.Forms.Label();
            this.MarketValueResult = new System.Windows.Forms.TextBox();
            this.Annual = new System.Windows.Forms.Label();
            this.CompanySearchedName = new System.Windows.Forms.Label();
            this.IssuedShares = new System.Windows.Forms.Label();
            this.TreasuryShares = new System.Windows.Forms.Label();
            this.OutstandingShares = new System.Windows.Forms.Label();
            this.IssuedSharesResult = new System.Windows.Forms.TextBox();
            this.TreasurySharesResult = new System.Windows.Forms.TextBox();
            this.OutstandingSharesResult = new System.Windows.Forms.TextBox();
            this.CurrentPrice = new System.Windows.Forms.Label();
            this.CurrentPriceResult = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // CompanyName
            // 
            this.CompanyName.Location = new System.Drawing.Point(12, 12);
            this.CompanyName.Name = "CompanyName";
            this.CompanyName.Size = new System.Drawing.Size(100, 25);
            this.CompanyName.TabIndex = 0;
            this.CompanyName.TextChanged += new System.EventHandler(this.CompanyName_TextChanged);
            // 
            // CompanyNameButton
            // 
            this.CompanyNameButton.Location = new System.Drawing.Point(118, 14);
            this.CompanyNameButton.Name = "CompanyNameButton";
            this.CompanyNameButton.Size = new System.Drawing.Size(75, 23);
            this.CompanyNameButton.TabIndex = 1;
            this.CompanyNameButton.Text = "Search";
            this.CompanyNameButton.UseVisualStyleBackColor = true;
            this.CompanyNameButton.Click += new System.EventHandler(this.CompanyNameButton_Click);
            // 
            // Result
            // 
            this.Result.AutoSize = true;
            this.Result.Location = new System.Drawing.Point(9, 98);
            this.Result.Name = "Result";
            this.Result.Size = new System.Drawing.Size(775, 45);
            this.Result.TabIndex = 2;
            this.Result.Text = "Result\n\n=========================================================================" +
    "=======================";
            this.Result.Click += new System.EventHandler(this.Result_Click);
            // 
            // Price80
            // 
            this.Price80.AutoSize = true;
            this.Price80.Location = new System.Drawing.Point(15, 167);
            this.Price80.Name = "Price80";
            this.Price80.Size = new System.Drawing.Size(106, 15);
            this.Price80.TabIndex = 3;
            this.Price80.Text = "Price if w = 0.8";
            this.Price80.Click += new System.EventHandler(this.Price80_Click);
            // 
            // Price80Result
            // 
            this.Price80Result.Location = new System.Drawing.Point(127, 164);
            this.Price80Result.Name = "Price80Result";
            this.Price80Result.Size = new System.Drawing.Size(179, 25);
            this.Price80Result.TabIndex = 4;
            this.Price80Result.TextChanged += new System.EventHandler(this.Price80Result_TextChanged);
            // 
            // StockPriceUnit
            // 
            this.StockPriceUnit.AutoSize = true;
            this.StockPriceUnit.Location = new System.Drawing.Point(312, 167);
            this.StockPriceUnit.Name = "StockPriceUnit";
            this.StockPriceUnit.Size = new System.Drawing.Size(32, 15);
            this.StockPriceUnit.TabIndex = 5;
            this.StockPriceUnit.Text = "Unit";
            this.StockPriceUnit.Click += new System.EventHandler(this.StockPriceUnit_Click);
            // 
            // Price90
            // 
            this.Price90.AutoSize = true;
            this.Price90.Location = new System.Drawing.Point(381, 167);
            this.Price90.Name = "Price90";
            this.Price90.Size = new System.Drawing.Size(106, 15);
            this.Price90.TabIndex = 6;
            this.Price90.Text = "Price if w = 0.9";
            // 
            // Price90Result
            // 
            this.Price90Result.Location = new System.Drawing.Point(502, 164);
            this.Price90Result.Name = "Price90Result";
            this.Price90Result.Size = new System.Drawing.Size(179, 25);
            this.Price90Result.TabIndex = 7;
            this.Price90Result.TextChanged += new System.EventHandler(this.Price90Result_TextChanged);
            // 
            // FairPrice
            // 
            this.FairPrice.AutoSize = true;
            this.FairPrice.Location = new System.Drawing.Point(15, 216);
            this.FairPrice.Name = "FairPrice";
            this.FairPrice.Size = new System.Drawing.Size(68, 15);
            this.FairPrice.TabIndex = 8;
            this.FairPrice.Text = "Fair Price";
            // 
            // FairPriceResult
            // 
            this.FairPriceResult.Location = new System.Drawing.Point(127, 213);
            this.FairPriceResult.Name = "FairPriceResult";
            this.FairPriceResult.Size = new System.Drawing.Size(179, 25);
            this.FairPriceResult.TabIndex = 9;
            this.FairPriceResult.TextChanged += new System.EventHandler(this.FairPriceResult_TextChanged);
            // 
            // ImportTXTStore
            // 
            this.ImportTXTStore.Location = new System.Drawing.Point(649, 12);
            this.ImportTXTStore.Name = "ImportTXTStore";
            this.ImportTXTStore.Size = new System.Drawing.Size(135, 60);
            this.ImportTXTStore.TabIndex = 10;
            this.ImportTXTStore.Text = "Import TXT And Export Excel";
            this.ImportTXTStore.UseVisualStyleBackColor = true;
            this.ImportTXTStore.Click += new System.EventHandler(this.ImportTXTStore_Click);
            // 
            // Detail
            // 
            this.Detail.AutoSize = true;
            this.Detail.Location = new System.Drawing.Point(9, 270);
            this.Detail.Name = "Detail";
            this.Detail.Size = new System.Drawing.Size(775, 45);
            this.Detail.TabIndex = 11;
            this.Detail.Text = "Detail\n\n=========================================================================" +
    "=======================";
            // 
            // ROE
            // 
            this.ROE.AutoSize = true;
            this.ROE.Location = new System.Drawing.Point(15, 354);
            this.ROE.Name = "ROE";
            this.ROE.Size = new System.Drawing.Size(38, 15);
            this.ROE.TabIndex = 12;
            this.ROE.Text = "ROE";
            // 
            // ROEResult
            // 
            this.ROEResult.Location = new System.Drawing.Point(127, 351);
            this.ROEResult.Name = "ROEResult";
            this.ROEResult.Size = new System.Drawing.Size(618, 25);
            this.ROEResult.TabIndex = 13;
            this.ROEResult.TextChanged += new System.EventHandler(this.ROEResult_TextChanged);
            // 
            // CompanyValue
            // 
            this.CompanyValue.AutoSize = true;
            this.CompanyValue.Location = new System.Drawing.Point(15, 452);
            this.CompanyValue.Name = "CompanyValue";
            this.CompanyValue.Size = new System.Drawing.Size(109, 15);
            this.CompanyValue.TabIndex = 14;
            this.CompanyValue.Text = "Company Value";
            // 
            // CompanyValueResult
            // 
            this.CompanyValueResult.Location = new System.Drawing.Point(127, 449);
            this.CompanyValueResult.Name = "CompanyValueResult";
            this.CompanyValueResult.Size = new System.Drawing.Size(222, 25);
            this.CompanyValueResult.TabIndex = 15;
            this.CompanyValueResult.TextChanged += new System.EventHandler(this.CompanyValueResult_TextChanged);
            // 
            // EquityOfControllingShareholders
            // 
            this.EquityOfControllingShareholders.AutoSize = true;
            this.EquityOfControllingShareholders.Location = new System.Drawing.Point(15, 394);
            this.EquityOfControllingShareholders.Name = "EquityOfControllingShareholders";
            this.EquityOfControllingShareholders.Size = new System.Drawing.Size(93, 45);
            this.EquityOfControllingShareholders.TabIndex = 16;
            this.EquityOfControllingShareholders.Text = "Equity Of\nControlling\nShareholders";
            // 
            // EquityOfControllingShareholdersResult
            // 
            this.EquityOfControllingShareholdersResult.Location = new System.Drawing.Point(127, 403);
            this.EquityOfControllingShareholdersResult.Name = "EquityOfControllingShareholdersResult";
            this.EquityOfControllingShareholdersResult.Size = new System.Drawing.Size(618, 25);
            this.EquityOfControllingShareholdersResult.TabIndex = 17;
            this.EquityOfControllingShareholdersResult.TextChanged += new System.EventHandler(this.EquityOfControllingShareholdersResult_TextChanged);
            // 
            // OriginalUnit
            // 
            this.OriginalUnit.AutoSize = true;
            this.OriginalUnit.Location = new System.Drawing.Point(751, 406);
            this.OriginalUnit.Name = "OriginalUnit";
            this.OriginalUnit.Size = new System.Drawing.Size(56, 30);
            this.OriginalUnit.TabIndex = 18;
            this.OriginalUnit.Text = "Original\nUnit";
            // 
            // ExportExcel
            // 
            this.ExportExcel.Location = new System.Drawing.Point(199, 14);
            this.ExportExcel.Name = "ExportExcel";
            this.ExportExcel.Size = new System.Drawing.Size(107, 51);
            this.ExportExcel.TabIndex = 19;
            this.ExportExcel.Text = "Search And Export Excel";
            this.ExportExcel.UseVisualStyleBackColor = true;
            this.ExportExcel.Click += new System.EventHandler(this.ExportExcel_Click);
            // 
            // NotKorea
            // 
            this.NotKorea.AutoSize = true;
            this.NotKorea.Location = new System.Drawing.Point(324, 18);
            this.NotKorea.Name = "NotKorea";
            this.NotKorea.Size = new System.Drawing.Size(93, 19);
            this.NotKorea.TabIndex = 20;
            this.NotKorea.Text = "Not Korea";
            this.NotKorea.UseVisualStyleBackColor = true;
            // 
            // MarketValue
            // 
            this.MarketValue.AutoSize = true;
            this.MarketValue.Location = new System.Drawing.Point(367, 452);
            this.MarketValue.Name = "MarketValue";
            this.MarketValue.Size = new System.Drawing.Size(90, 15);
            this.MarketValue.TabIndex = 21;
            this.MarketValue.Text = "Market Value";
            this.MarketValue.Click += new System.EventHandler(this.MarketValue_Click);
            // 
            // MarketValueResult
            // 
            this.MarketValueResult.Location = new System.Drawing.Point(482, 449);
            this.MarketValueResult.Name = "MarketValueResult";
            this.MarketValueResult.Size = new System.Drawing.Size(263, 25);
            this.MarketValueResult.TabIndex = 22;
            this.MarketValueResult.TextChanged += new System.EventHandler(this.MarketValueResult_TextChanged);
            // 
            // Annual
            // 
            this.Annual.AutoSize = true;
            this.Annual.Location = new System.Drawing.Point(124, 324);
            this.Annual.Name = "Annual";
            this.Annual.Size = new System.Drawing.Size(51, 15);
            this.Annual.TabIndex = 23;
            this.Annual.Text = "Annual";
            // 
            // CompanySearchedName
            // 
            this.CompanySearchedName.AutoSize = true;
            this.CompanySearchedName.Location = new System.Drawing.Point(13, 56);
            this.CompanySearchedName.Name = "CompanySearchedName";
            this.CompanySearchedName.Size = new System.Drawing.Size(110, 15);
            this.CompanySearchedName.TabIndex = 24;
            this.CompanySearchedName.Text = "Company Name";
            // 
            // IssuedShares
            // 
            this.IssuedShares.AutoSize = true;
            this.IssuedShares.Location = new System.Drawing.Point(15, 490);
            this.IssuedShares.Name = "IssuedShares";
            this.IssuedShares.Size = new System.Drawing.Size(101, 15);
            this.IssuedShares.TabIndex = 25;
            this.IssuedShares.Text = "Issued Shares";
            // 
            // TreasuryShares
            // 
            this.TreasuryShares.AutoSize = true;
            this.TreasuryShares.Location = new System.Drawing.Point(367, 490);
            this.TreasuryShares.Name = "TreasuryShares";
            this.TreasuryShares.Size = new System.Drawing.Size(114, 15);
            this.TreasuryShares.TabIndex = 26;
            this.TreasuryShares.Text = "Treasury Shares";
            // 
            // OutstandingShares
            // 
            this.OutstandingShares.AutoSize = true;
            this.OutstandingShares.Location = new System.Drawing.Point(15, 530);
            this.OutstandingShares.Name = "OutstandingShares";
            this.OutstandingShares.Size = new System.Drawing.Size(86, 30);
            this.OutstandingShares.TabIndex = 27;
            this.OutstandingShares.Text = "Outstanding\nShares";
            // 
            // IssuedSharesResult
            // 
            this.IssuedSharesResult.Location = new System.Drawing.Point(127, 487);
            this.IssuedSharesResult.Name = "IssuedSharesResult";
            this.IssuedSharesResult.Size = new System.Drawing.Size(222, 25);
            this.IssuedSharesResult.TabIndex = 28;
            // 
            // TreasurySharesResult
            // 
            this.TreasurySharesResult.Location = new System.Drawing.Point(482, 487);
            this.TreasurySharesResult.Name = "TreasurySharesResult";
            this.TreasurySharesResult.Size = new System.Drawing.Size(263, 25);
            this.TreasurySharesResult.TabIndex = 29;
            // 
            // OutstandingSharesResult
            // 
            this.OutstandingSharesResult.Location = new System.Drawing.Point(127, 530);
            this.OutstandingSharesResult.Name = "OutstandingSharesResult";
            this.OutstandingSharesResult.Size = new System.Drawing.Size(618, 25);
            this.OutstandingSharesResult.TabIndex = 30;
            // 
            // CurrentPrice
            // 
            this.CurrentPrice.AutoSize = true;
            this.CurrentPrice.Location = new System.Drawing.Point(381, 216);
            this.CurrentPrice.Name = "CurrentPrice";
            this.CurrentPrice.Size = new System.Drawing.Size(86, 15);
            this.CurrentPrice.TabIndex = 31;
            this.CurrentPrice.Text = "Current Price";
            // 
            // CurrentPriceResult
            // 
            this.CurrentPriceResult.Location = new System.Drawing.Point(502, 213);
            this.CurrentPriceResult.Name = "CurrentPriceResult";
            this.CurrentPriceResult.Size = new System.Drawing.Size(179, 25);
            this.CurrentPriceResult.TabIndex = 32;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 620);
            this.Controls.Add(this.CurrentPriceResult);
            this.Controls.Add(this.CurrentPrice);
            this.Controls.Add(this.OutstandingSharesResult);
            this.Controls.Add(this.TreasurySharesResult);
            this.Controls.Add(this.IssuedSharesResult);
            this.Controls.Add(this.OutstandingShares);
            this.Controls.Add(this.TreasuryShares);
            this.Controls.Add(this.IssuedShares);
            this.Controls.Add(this.CompanySearchedName);
            this.Controls.Add(this.Annual);
            this.Controls.Add(this.MarketValueResult);
            this.Controls.Add(this.MarketValue);
            this.Controls.Add(this.NotKorea);
            this.Controls.Add(this.ExportExcel);
            this.Controls.Add(this.OriginalUnit);
            this.Controls.Add(this.EquityOfControllingShareholdersResult);
            this.Controls.Add(this.EquityOfControllingShareholders);
            this.Controls.Add(this.CompanyValueResult);
            this.Controls.Add(this.CompanyValue);
            this.Controls.Add(this.ROEResult);
            this.Controls.Add(this.ROE);
            this.Controls.Add(this.Detail);
            this.Controls.Add(this.ImportTXTStore);
            this.Controls.Add(this.FairPriceResult);
            this.Controls.Add(this.FairPrice);
            this.Controls.Add(this.Price90Result);
            this.Controls.Add(this.Price90);
            this.Controls.Add(this.Price80Result);
            this.Controls.Add(this.Price80);
            this.Controls.Add(this.Result);
            this.Controls.Add(this.CompanyNameButton);
            this.Controls.Add(this.CompanyName);
            this.Controls.Add(this.StockPriceUnit);
            this.Name = "Form1";
            this.Text = "Stock Price Calculator";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox CompanyName;
        private System.Windows.Forms.Button CompanyNameButton;
        private System.Windows.Forms.Label Result;
        private System.Windows.Forms.Label Price80;
        private System.Windows.Forms.TextBox Price80Result;
        private System.Windows.Forms.Label StockPriceUnit;
        private System.Windows.Forms.Label Price90;
        private System.Windows.Forms.TextBox Price90Result;
        private System.Windows.Forms.Label FairPrice;
        private System.Windows.Forms.TextBox FairPriceResult;
        private System.Windows.Forms.Button ImportTXTStore;
        private System.Windows.Forms.Label Detail;
        private System.Windows.Forms.Label ROE;
        private System.Windows.Forms.TextBox ROEResult;
        private System.Windows.Forms.Label CompanyValue;
        private System.Windows.Forms.TextBox CompanyValueResult;
        private System.Windows.Forms.Label EquityOfControllingShareholders;
        private System.Windows.Forms.TextBox EquityOfControllingShareholdersResult;
        private System.Windows.Forms.Label OriginalUnit;
        private System.Windows.Forms.Button ExportExcel;
        private System.Windows.Forms.CheckBox NotKorea;
        private System.Windows.Forms.Label MarketValue;
        private System.Windows.Forms.TextBox MarketValueResult;
        private System.Windows.Forms.Label Annual;
        private System.Windows.Forms.Label CompanySearchedName;
        private System.Windows.Forms.Label IssuedShares;
        private System.Windows.Forms.Label TreasuryShares;
        private System.Windows.Forms.Label OutstandingShares;
        private System.Windows.Forms.TextBox IssuedSharesResult;
        private System.Windows.Forms.TextBox TreasurySharesResult;
        private System.Windows.Forms.TextBox OutstandingSharesResult;
        private System.Windows.Forms.Label CurrentPrice;
        private System.Windows.Forms.TextBox CurrentPriceResult;
    }
}

