# CLAUDE.md - Application_TradeWin

Metal weighbridge and trading app (TradeWin Metals): two-weight receiving and despatch, public weighing, scale weighing, tickets, invoices with an e-invoice panel and export, control tables, reports, stock adjustment. VC6/MFC on wlib32, project `menu.dsp`, exe `menu.exe`. Member of the SengHiap family (base: Application_SengHiap_Metal; closest sibling: Application_LekokPaper), with a reduced e-invoice enhancement (commit "E-Invoice enhancement", program title `VER 2.00`), no cashier, camera or sensor code.

Module reference (read it first for any question about screens, tables, flows or reports):
[docs/module-reference/README.md](docs/module-reference/README.md)

**Rule:** Any change to behaviour, tables, fields, screens, reports or calculations must update the module reference (English, plus zh-CN when the changed section exists there) in the same commit.

Facts worth knowing before editing:
- `menu.rc` has one menu resource, `IDR_MAINMENU` (35 items), created in `CMenuApp::Domain` (`MENU.CPP`). Menu ID `ID000nn` is node `nn`, handled by `CMenuApp::SubProgram` in `MAIN.CPP`; the caption number does not always equal the node (2.3 is node 26). 6.4 Reset Database (node 64) really deletes data; 6.5 E-Invoice Profile (node 65) has no password check.
- All tables are opened in `MENU.CPP` (`OpenAllDataFile`). Field enums are in `MENU.H` and must match the SQL column order. There is no schema repo and no `.sql` file: check the live database.
- `menu.dsp` compiles 118 of the 142 `.cpp` files; check the project before assuming a file is live. The accounting code (AR/AP, GL) is compiled but has no menu path.
- The e-invoice panel (`CEINV_Functions`), the account e-invoice window (`CEinv`) and the company profile screen (`CCmprofile`) live in the shared wlib32 library, not in this repo; their SQL procedures and tables are not in any script.
- The weight is read through the wlib32 comm layer; a Debug build simulates it (GETWT.CPP), never ship one.
- The shared libraries wlib32, winlib and wtlib32 are not edited from this repo.
- Never put passwords, connection data or the Reset Database password rule in docs or code comments. `PROVIDER.SYS` holds connection data (a login on each line, and a plain-text password on the first).
- Build and debug with the `vc6-build-debug` skill; document with the `domain-module-reference` skill; framework questions with the `wlib32-kb` skill.
