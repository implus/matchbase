--- ex-1: offline: 3.3%(2000) online: 1.9%
cos|tfid|c->(prob(a)xprob(b))|

--- ex00: offline: 3.65% online: 1.49%
cos | tfidf | c->(max(a), b) |

--- ex01: offline: 4.516%(4135)  online: (4574 + 3.8onlinebest)
cos | tfidf | bought history + c->(max(a), b) |






--- ex04: offline:
cos | tfidf | bought history + c->(prob(a)xprob(b)) |

--- ex05: offline:
cos | tfidf | bought history - 1 times + c->(prob(a)xprob(b)) |




 ./gao ../data/dim_items.txt ../init/offline_train_item_item.txt ../data/offline_test_items.txt offline_ans.txt

------------------------------------ belows are no use ------------------------



--- ex02: offline: 3.8%(1500)
dot multiply + tfidf + bought history + c ->(max(a), b) b's same list 


--- ex03: offline: 2.xx%
dot multiply + tfidf + c ->(max(a), b) b's same list
