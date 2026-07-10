/**
 * Google Apps Script — Attendance Logger
 * ---------------------------------------
 * Deploy this as a Web App (Deploy > New deployment > Web app):
 *   - Execute as: Me
 *   - Who has access: Anyone
 * Then copy the deployment URL into config.h as scriptURL.
 *
 * Expects GET requests like:
 *   .../exec?name=RAJAT&status=IN
 */

function doGet(e) {
  const sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();

  const name = e.parameter.name || "Unknown";
  const status = e.parameter.status || "Unknown";
  const timestamp = new Date();

  sheet.appendRow([timestamp, name, status]);

  return ContentService
    .createTextOutput("OK")
    .setMimeType(ContentService.MimeType.TEXT);
}
