# Application_Expert_Plastic

Weighbridge program of Expert Plastic Sdn Bhd: VC6/MFC app on wlib32, winlib, wtlib32 (menus Weighing, Adjustment/Invoicing, Table, Report, Utility). Default branch is `main`. Build: `menu.dsp`, exe `menu.exe`.

Module reference (business view and technical view): [docs/MODULE_REFERENCE.md](docs/MODULE_REFERENCE.md)

Rule: Any change to behaviour, tables, fields, screens, reports or calculations must update the module reference (English, plus zh-CN when the changed section exists there) in the same commit.

- Same file-name family as Application_WangLian but an older, different code line; see "Differences from Application_WangLian" in the reference.
- No schema repo and no .sql files: column types in the reference are hints or unverified.
- Never write credentials into docs. `PROVIDER.SYS`, `DeviceCfg.json` and some code hold passwords: name the file and line only.
- Check a doc change with `py -I <skills>\domain-module-reference\scripts\check_doc.py <repo>` and `check_mermaid.mjs`.
