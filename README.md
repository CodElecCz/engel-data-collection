# ENGEL Data Collection

Articles: [Česky](blog/index.cs.md) | [English](blog/index.en.md) | [Website](https://www.codelec.cz/blog/engel-data-collection)

## Project overview

ENGEL Data Collection is a C++17 and Qt desktop application that monitors machine report files and imports new production records into a SQL database. It watches a configured `ReportDataIMM.dat` file for each machine, keeps a view of file status and processing progress, and provides a database browser for inspecting tables and running SQL queries.

The application is built with Qt Core, Gui, Widgets, and SQL. Its main components are:

- `file/FileView.*` configures and presents the monitored files.
- `file/FileCheckModel.*` and `file/FileCheck.*` check file changes, parse new report rows, and insert them into the database.
- `browser/` provides SQL connection management and a query browser.
- `settings/` reads application configuration and stores the last imported timestamp for each machine.
- `log/` provides the in-app and file-based application log.

The source report contains fields such as `DATE`, `TIME`, `COUNT`, `SetDescPrt`, `ActCntMld`, `SetDescMld`, `ActCntCyc`, `ActTimCyc`, and `ActStsMach`. Imported rows include the configured machine ID and a normalized `datetime` in the `reportdataimm` table.

## Configuration

The application reads an INI file beside the executable. Configure the SQL connection, machine directory, machine IDs, report-file suffix, polling interval, and whether checking starts automatically. Do not commit deployment-specific connection settings or credentials.

## Build

Open `EngelDataCollection.pro` in Qt Creator with Qt Core, Gui, Widgets, and SQL modules installed. The user manual documents a Qt 5.14.2 / MSVC 2017 build environment.

## Articles

- [Project overview in Czech](blog/index.cs.md)
- [Project overview in English](blog/index.en.md)
