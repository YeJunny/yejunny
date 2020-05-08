using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using HtmlAgilityPack;
using OpenQA.Selenium;
using OpenQA.Selenium.Chrome;
using OpenQA.Selenium.Support.UI;

namespace StockPriceCalculator
{
    public partial class Form1 : Form
    {
        private const string tab = "    ";
        private const string blank = "&nbsp;";
        private IWebDriver mDriver;

        public Form1()
        {
            InitializeComponent();
        }

        private void CompanyNameButton_Click(object sender, EventArgs e)
        {
            if (NotKorea.Checked)
            {
                mDriver = new ChromeDriver();
                mDriver.Navigate().GoToUrl("https://www.macrotrends.net/");
                WebDriverWait wait = new WebDriverWait(mDriver, new TimeSpan(0, 0, 10));
                try
                {
                    wait.Until(SeleniumExtras.WaitHelpers.ExpectedConditions.ElementIsVisible(By.XPath("/html/body/div[1]/div[2]/div[2]/div[2]/div/form/div[1]/div[1]/span[1]/input")));
                    mDriver.FindElement(By.XPath("/html/body/div[1]/div[2]/div[2]/div[2]/div/form/div[1]/div[1]/span[1]/input")).SendKeys(CompanyName.Text);
                    wait.Until(SeleniumExtras.WaitHelpers.ExpectedConditions.ElementIsVisible(By.XPath("/html/body/div[1]/div[2]/div[2]/div[2]/div/form/div[1]/div[2]/ul/li[1]/a")));
                }
                catch (Exception exception)
                {
                    mDriver.Close();
                    mDriver.Quit();
                    MessageBox.Show($"{exception.Message}");
                    return;
                }

                string infoUrl = mDriver.FindElement(By.XPath("/html/body/div[1]/div[2]/div[2]/div[2]/div/form/div[1]/div[2]/ul/li[1]/a")).GetAttribute("href");
                infoUrl = infoUrl.Substring(0, infoUrl.Length - 8);

                // ROE
                mDriver.Navigate().GoToUrl($"{infoUrl}/financial-ratios");
                string[] roe = new string[4];
                float[] roeFloat = new float[4];
                wait.Until(SeleniumExtras.WaitHelpers.ExpectedConditions.ElementIsVisible(By.XPath("/html/body/div[2]/div[3]/div[3]/div/div/div[4]/div[2]/div/div[14]/div[1]/div/a")));
                string roeCheckString = mDriver.FindElement(By.XPath("/html/body/div[2]/div[3]/div[3]/div/div/div[4]/div[2]/div/div[14]/div[1]/div/a")).Text;
                if (roeCheckString == "ROE - Return On Equity")
                {
                    for (int i = 0; i < 4; ++i)
                    {
                        wait.Until(SeleniumExtras.WaitHelpers.ExpectedConditions.ElementIsVisible(By.XPath("/html/body/div[2]/div[3]/div[3]/div/div/div[4]/div[2]/div/div[14]/div[" + (i + 3).ToString() + "]/div")));
                        roe[i] = mDriver.FindElement(By.XPath("/html/body/div[2]/div[3]/div[3]/div/div/div[4]/div[2]/div/div[14]/div[" + (i + 3).ToString() + "]/div")).Text;
                        float.TryParse(roe[i], out roeFloat[i]);
                    }
                    ROEResult.Text = $"{roe[3]} {tab}{tab}{tab}{tab}{tab} {roe[2]} {tab}{tab}{tab}{tab}{tab} {roe[1]} {tab}{tab}{tab}{tab}{tab} {roe[0]}";
                }
                else
                {
                    MessageBox.Show($"ROE Error");
                    return;
                }

                // Years
                mDriver.Navigate().GoToUrl($"{infoUrl}/balance-sheet");
                string[] years = new string[4];
                for (int i = 0; i < 4; ++i)
                {
                    wait.Until(SeleniumExtras.WaitHelpers.ExpectedConditions.ElementIsVisible(By.XPath("/html/body/div[2]/div[3]/div[3]/div/div/div[4]/div[1]/div/div[" + (i + 3).ToString() + "]/div/div[1]/span")));
                    years[i] = mDriver.FindElement(By.XPath("/html/body/div[2]/div[3]/div[3]/div/div/div[4]/div[1]/div/div[" + (i + 3).ToString() + "]/div/div[1]/span")).Text;
                }
                Annual.Text = $"{years[3]} {tab}{tab}{tab}{tab} {years[2]} {tab}{tab}{tab}{tab} {years[1]} {tab}{tab}{tab}{tab} {years[0]}";

                // Equity Of Controlling Shareholders
                string[] equitys = new string[4];
                for (int i = 0; i < 4; ++i)
                {
                    wait.Until(SeleniumExtras.WaitHelpers.ExpectedConditions.ElementIsVisible(By.XPath("/html/body/div[2]/div[3]/div[3]/div/div/div[4]/div[2]/div/div[21]/div[" + (i + 3).ToString() + "]/div")));
                    equitys[i] = mDriver.FindElement(By.XPath("/html/body/div[2]/div[3]/div[3]/div/div/div[4]/div[2]/div/div[21]/div[" + (i + 3).ToString() + "]/div")).Text;
                }
                EquityOfControllingShareholdersResult.Text = $"{equitys[3]} {tab}{tab}{tab}{tab} {equitys[2]} {tab}{tab}{tab}{tab} {equitys[1]} {tab}{tab}{tab}{tab} {equitys[0]}";


                mDriver.Close();
                mDriver.Quit();

                HtmlWeb htmlDoc = new HtmlWeb();

                // Enterprise Bond Ratio
                HtmlAgilityPack.HtmlDocument doc = htmlDoc.Load($"https://www.investing.com/search/?q={CompanyName.Text}");
                string companySymbol = doc.DocumentNode.SelectSingleNode("/html/body/div[5]/section/div/div[2]/div[2]/div[1]/a[1]/span[2]").InnerText;
                CompanySearchedName.Text = doc.DocumentNode.SelectSingleNode("/html/body/div[5]/section/div/div[2]/div[2]/div[1]/a[1]/span[3]").InnerText;
                doc = htmlDoc.Load("https://ycharts.com/indicators/us_corporate_bbb_effective_yield");
                float CompanyBondRatio = float.Parse(doc.GetElementbyId("pgNameVal").InnerText.Split('%')[0]);

                string financialUrl = $"https://finance.yahoo.com/quote/{companySymbol}";
                doc = htmlDoc.Load(financialUrl);

                // Current Stock Price
                CurrentPriceResult.Text = doc.DocumentNode.SelectSingleNode("/html/body/div[1]/div/div/div[1]/div/div[2]/div/div/div[4]/div/div/div/div[3]/div[1]/div/span[1]").InnerText;

                // Market Value
                MarketValueResult.Text = doc.DocumentNode.SelectSingleNode("/html/body/div[1]/div/div/div[1]/div/div[3]/div[1]/div/div[1]/div/div/div/div[2]/div[2]/table/tbody/tr[1]/td[2]/span").InnerText;

                // Outstanding Shares
                doc = htmlDoc.Load($"{infoUrl}/shares-outstanding");
                string UnitString = doc.DocumentNode.SelectSingleNode("/html/body/div[2]/div[4]/div[7]/div[1]/table/thead/tr/th/span").InnerText;
                decimal OutstandingSharesValue;
                if (UnitString == "(Millions of Shares)")
                {
                    string OutstandingYear = doc.DocumentNode.SelectSingleNode("/html/body/div[2]/div[4]/div[7]/div[1]/table/tbody/tr[1]/td[1]").InnerText;
                    OutstandingSharesValue = decimal.Parse(doc.DocumentNode.SelectSingleNode("/html/body/div[2]/div[4]/div[7]/div[1]/table/tbody/tr[1]/td[2]").InnerText) * (decimal)1000000;
                    OutstandingShares.Text = $"Outstanding\nShares({OutstandingYear})";
                    OutstandingSharesResult.Text = $"{OutstandingSharesValue.ToString("N2")}";
                    IssuedSharesResult.Text = $"";
                    TreasurySharesResult.Text = $"";
                }
                else
                {
                    MessageBox.Show($"Outstanding Shares Error");
                    return;
                }

                // Detail Setting
                float roeValue;
                decimal equity;
                string currentYear = DateTime.Now.ToString("yyyy");
                int DisclosureYear = int.Parse(years[0].Split('-')[0]);
                roeValue = (roeFloat[3] + roeFloat[2] * 2 + roeFloat[1] * 3 + roeFloat[0] * 4) / 10.0f;
                equity = decimal.Parse(equitys[0], System.Globalization.NumberStyles.Currency);
                Annual.Text += $" {tab}{tab}{tab}{tab} {DisclosureYear + 1}";
                ROEResult.Text += $" {tab}{tab}{tab}{tab}{tab} {roeValue}";
                CompanySearchedName.Text += $"({DisclosureYear})";

                // Company Value
                decimal CompanyValue = equity + (equity * (decimal)((roeValue - CompanyBondRatio) / CompanyBondRatio));
                CompanyValueResult.Text = Math.Round(CompanyValue, 2).ToString("N2");
                FairPriceResult.Text = Math.Round(CompanyValue / OutstandingSharesValue * (decimal)1000000.0, 2).ToString("N2");
                decimal ExceedProfit = equity * (decimal)((roeValue - CompanyBondRatio) / 100.0);
                if (ExceedProfit > (decimal)0.0)
                {
                    decimal CompanyValue80 = equity + (ExceedProfit * (decimal)(0.8 / (1.0 + CompanyBondRatio / 100.0 - 0.8)));
                    decimal CompnayValue90 = equity + (ExceedProfit * (decimal)(0.9 / (1.0 + CompanyBondRatio / 100.0 - 0.9)));
                    Price80Result.Text = Math.Round(CompanyValue80 / OutstandingSharesValue * (decimal)1000000.0, 2).ToString("N2");
                    Price90Result.Text = Math.Round(CompnayValue90 / OutstandingSharesValue * (decimal)1000000.0, 2).ToString("N2");
                }
                OriginalUnit.Text = "M Dollar";
                StockPriceUnit.Text = "Dollar";

                MessageBox.Show($"Complete!");
            }
            else
            {
                // Search Company
                mDriver = new ChromeDriver();
                mDriver.Navigate().GoToUrl("http://comp.fnguide.com/SVO2/common/lookup.asp");
                WebDriverWait wait = new WebDriverWait(mDriver, new TimeSpan(0, 0, 10));
                try
                {
                    wait.Until(SeleniumExtras.WaitHelpers.ExpectedConditions.ElementIsVisible(By.Id("txtSearchKey")));
                    wait.Until(SeleniumExtras.WaitHelpers.ExpectedConditions.ElementIsVisible(By.ClassName("us_btn_ty1")));
                    wait.Until(SeleniumExtras.WaitHelpers.ExpectedConditions.ElementIsVisible(By.ClassName("l")));
                }
                catch (Exception exception)
                {
                    mDriver.Close();
                    mDriver.Quit();
                    MessageBox.Show($"{exception.Message}");
                    return;
                }

                string firstCompanyName = mDriver.FindElement(By.ClassName("l")).Text;
                mDriver.FindElement(By.Id("txtSearchKey")).SendKeys(CompanyName.Text);
                mDriver.FindElement(By.ClassName("us_btn_ty1")).Click();
                
                wait.Until(condition =>
                {
                    try
                    {
                        wait.Until(SeleniumExtras.WaitHelpers.ExpectedConditions.ElementIsVisible(By.XPath("/html/body/div/div/div[2]/div/div[3]/div/table/tbody/tr/td[2]")));
                        IWebElement element = mDriver.FindElement(By.XPath("/html/body/div/div/div[2]/div/div[3]/div/table/tbody/tr/td[2]"));
                        if (element.Text == firstCompanyName)
                        {
                            return false;
                        }
                        else
                        {
                            return true;
                        }
                    }
                    catch (Exception exception)
                    {
                        MessageBox.Show($"{exception.Message}");
                        return true;
                    }
                });

                string companySymbol = "조회 결과가 없습니다.";
                string companyElementStr = mDriver.FindElement(By.XPath("/html/body/div/div/div[2]/div/div[3]/div/table/tbody")).Text;
                string[] collectionNames = companyElementStr.Split('\n');
                for (int i = 0; i < collectionNames.Length; ++i)
                {
                    if (collectionNames[i].Contains(CompanyName.Text))
                    {
                        CompanySearchedName.Text = mDriver.FindElement(By.XPath("/html/body/div/div/div[2]/div/div[3]/div/table/tbody/tr[" + (i + 1).ToString() + "]/td[2]")).Text;
                        companySymbol = mDriver.FindElement(By.XPath("/html/body/div/div/div[2]/div/div[3]/div/table/tbody/tr[" + (i + 1).ToString() + "]/td[1]")).Text;
                        break;
                    }
                }
                mDriver.Close();
                mDriver.Quit();
                if (companySymbol == "조회 결과가 없습니다.")
                {
                    MessageBox.Show("Not Found!");
                    return;
                }

                HtmlWeb htmlDoc = new HtmlWeb();

                // Enterprise Bond Ratio
                HtmlAgilityPack.HtmlDocument doc = htmlDoc.Load("https://www.kisrating.com/ratingsStatistics/statics_spread.do");
                string BBB = doc.DocumentNode.SelectSingleNode("/html/body/div[2]/div[3]/div[2]/div[2]/div[2]/table/tbody/tr[11]/td[1]").InnerText;
                float CompanyBondRatio;
                if (BBB == "BBB-")
                {
                    CompanyBondRatio = float.Parse(doc.DocumentNode.SelectSingleNode("/html/body/div[2]/div[3]/div[2]/div[2]/div[2]/table/tbody/tr[11]/td[9]").InnerText);
                }
                else
                {
                    MessageBox.Show("BBB- Error");
                    return;
                }

                string krFinancialStatementsUrl = "http://comp.fnguide.com/SVO2/ASP/SVD_main.asp?pGB=1&gicode=" + companySymbol;
                doc = htmlDoc.Load(krFinancialStatementsUrl);

                // Current Stock Price
                string currentStockPrice = doc.DocumentNode.SelectSingleNode("/html/body/div[2]/div/div/div[3]/div[1]/div[2]/table/tbody/tr[1]/td[1]").InnerText;
                CurrentPriceResult.Text = $"{currentStockPrice.Split('/')[0]}";

                // Market Value
                MarketValueResult.Text = doc.DocumentNode.SelectSingleNode("/html/body/div[2]/div/div/div[3]/div[1]/div[2]/table/tbody/tr[5]/td[1]").InnerText;

                // Issued Shares
                string issuedShares = doc.DocumentNode.SelectSingleNode("/html/body/div[2]/div/div/div[3]/div[1]/div[2]/table/tbody/tr[7]/td[1]").InnerText;
                decimal issuedSharesValue = decimal.Parse(issuedShares.Split('/')[0], System.Globalization.NumberStyles.Currency);

                // Treasury Shares
                string TreasuryShares = doc.DocumentNode.SelectSingleNode("/html/body/div[2]/div/div/div[3]/div[4]/div[2]/div/div[2]/table/tbody/tr[5]/td[2]").InnerText;
                decimal OutstandingShares;
                if (TreasuryShares == blank)
                {
                    OutstandingShares = issuedSharesValue;
                }
                else
                {
                    OutstandingShares = issuedSharesValue - decimal.Parse(TreasuryShares, System.Globalization.NumberStyles.Currency);
                }
                IssuedSharesResult.Text = $"{issuedSharesValue.ToString("N2")}";
                TreasurySharesResult.Text = $"{TreasuryShares}";
                OutstandingSharesResult.Text = $"{OutstandingShares.ToString("N2")}";

                // Years
                string[] years = new string[4];
                for (int i = 0; i < 4; ++i)
                {
                    years[i] = doc.DocumentNode.SelectSingleNode("/html/body/div[2]/div/div/div[3]/div[13]/div[2]/table/thead/tr[2]/th[" + (i + 1).ToString() + "]/div").InnerText;
                }
                if (years[3].Length > 10)
                {
                    years[3] = doc.DocumentNode.SelectSingleNode("/html/body/div[2]/div/div/div[3]/div[13]/div[2]/table/thead/tr[2]/th[4]/div/a/span").InnerText;
                }
                Annual.Text = $"{years[0]} {tab}{tab}{tab}{tab} {years[1]} {tab}{tab}{tab}{tab} {years[2]} {tab}{tab}{tab}{tab} {years[3]}";

                // Equity Of Controlling Shareholders
                string[] equitys = new string[4];
                for (int i = 0; i < 4; ++i)
                {
                    equitys[i] = doc.DocumentNode.SelectSingleNode("/html/body/div[2]/div/div/div[3]/div[13]/div[2]/table/tbody/tr[10]/td[" + (i + 1).ToString() + "]").InnerText;
                }
                EquityOfControllingShareholdersResult.Text = $"{equitys[0]} {tab}{tab}{tab}{tab} {equitys[1]} {tab}{tab}{tab}{tab} {equitys[2]} {tab}{tab}{tab}{tab} {equitys[3]}";

                // ROE
                string[] roe = new string[4];
                float[] roeFloat = new float[4];
                for (int i = 0; i < 4; ++i)
                {
                    roe[i] = doc.DocumentNode.SelectSingleNode("/html/body/div[2]/div/div/div[3]/div[13]/div[2]/table/tbody/tr[18]/td[" + (i + 1).ToString() + "]").InnerText;
                    float.TryParse(roe[i], out roeFloat[i]);
                }
                ROEResult.Text = $"{roe[0]} {tab}{tab}{tab}{tab}{tab} {roe[1]} {tab}{tab}{tab}{tab}{tab} {roe[2]} {tab}{tab}{tab}{tab}{tab} {roe[3]}";

                // Detail Setting
                float roeValue;
                decimal equity;
                string currentYear = DateTime.Now.ToString("yyyy");
                int DisclosureYear = int.Parse(years[3].Split('/')[0]);
                if (roe[3] != blank && equitys[3] != blank)
                {
                    roeValue = (roeFloat[0] + roeFloat[1] * 2 + roeFloat[2] * 3 + roeFloat[3] * 4) / 10.0f;
                    equity = decimal.Parse(equitys[3], System.Globalization.NumberStyles.Currency);
                    Annual.Text += $" {tab}{tab}{tab}{tab} {DisclosureYear + 1}";
                    ROEResult.Text += $" {tab}{tab}{tab}{tab}{tab} {roeValue}";
                    CompanySearchedName.Text += $"({DisclosureYear})";
                }
                else
                {
                    roeValue = (roeFloat[0] + roeFloat[1] * 2 + roeFloat[2] * 3) / 6.0f;
                    equity = decimal.Parse(equitys[2], System.Globalization.NumberStyles.Currency);
                    ROEResult.Text += $"({roeValue.ToString()})";
                    CompanySearchedName.Text += $"({DisclosureYear - 1})";
                }

                // Company Value
                decimal CompanyValue = equity + (equity * (decimal)((roeValue - CompanyBondRatio) / CompanyBondRatio));
                CompanyValueResult.Text = Math.Round(CompanyValue, 2).ToString("N2");
                FairPriceResult.Text = Math.Round(CompanyValue / OutstandingShares * (decimal)100000000.0, 2).ToString("N2");
                decimal ExceedProfit = equity * (decimal)((roeValue - CompanyBondRatio) / 100.0);
                if (ExceedProfit > (decimal)0.0)
                {
                    decimal CompanyValue80 = equity + (ExceedProfit * (decimal)(0.8 / (1.0 + CompanyBondRatio / 100.0 - 0.8)));
                    decimal CompnayValue90 = equity + (ExceedProfit * (decimal)(0.9 / (1.0 + CompanyBondRatio / 100.0 - 0.9)));
                    Price80Result.Text = Math.Round(CompanyValue80 / OutstandingShares * (decimal)100000000.0, 2).ToString("N2");
                    Price90Result.Text = Math.Round(CompnayValue90 / OutstandingShares * (decimal)100000000.0, 2).ToString("N2");
                }
                OriginalUnit.Text = "억원";
                StockPriceUnit.Text = "원";

                MessageBox.Show($"Complete!");
            }
        }

        private void Result_Click(object sender, EventArgs e)
        {

        }

        private void maskedPrice90Result_MaskInputRejected(object sender, MaskInputRejectedEventArgs e)
        {

        }

        private void StockPriceUnit_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Price80Result_TextChanged(object sender, EventArgs e)
        {

        }

        private void Price80_Click(object sender, EventArgs e)
        {

        }

        private void ExportExcel_Click(object sender, EventArgs e)
        {

        }

        private void ImportTXTStore_Click(object sender, EventArgs e)
        {

        }

        private void Price90Result_TextChanged(object sender, EventArgs e)
        {

        }

        private void FairPriceResult_TextChanged(object sender, EventArgs e)
        {

        }

        private void ROEResult_TextChanged(object sender, EventArgs e)
        {

        }

        private void EquityOfControllingShareholdersResult_TextChanged(object sender, EventArgs e)
        {

        }

        private void CompanyValueResult_TextChanged(object sender, EventArgs e)
        {

        }

        private void CompanyName_TextChanged(object sender, EventArgs e)
        {

        }

        private void MarketValue_Click(object sender, EventArgs e)
        {

        }

        private void MarketValueResult_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
