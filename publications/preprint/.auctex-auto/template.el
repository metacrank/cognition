(TeX-add-style-hook
 "template"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("LTJournalArticle" "	a4paper" "	10pt" "		twoside" "")))
   (TeX-run-style-hooks
    "latex2e"
    "LTJournalArticle"
    "LTJournalArticle10")
   (LaTeX-add-labels
    "eq:example"
    "tab:distcounts"
    "fig:tcanther"
    "fig:bpartery")
   (LaTeX-add-bibliographies
    "sample"))
 :latex)

