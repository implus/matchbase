--- ex-1: offline: 3.3%(2000) online: 1.9%
cos|tfid|c->(prob(a)xprob(b))|

--- ex00: offline: 3.65% online: 1.49%
cos | tfidf | c->(max(a), b) |

--- ex01: offline: 4.516%(4135)  online: (4574 + 3.8onlinebest)
cos | tfidf | bought history + c->(max(a), b) |






--- ex04: offline: 4.67%(4135)   online: 4.02%
cos | tfidf | bought history + c->(prob(a)xprob(b)) |

--- ex05: offline: 4.967%(4135)  online: 4.00%
cos | tfidf | bought history - 1 times(max==1, ignore) + c->(prob(a)xprob(b)) |

--- ex06: offline: 4.947%(4135)
cos | tfidf | bought history - 1 times(max==1, ignore) + c->(prob(a)xprob(b)) | erase top 5 values

--- ex07: offline: 5.110%(4135)
cos | tfidf |  bought history - 1 + c->(prob(a)xprob(b)) | 

--- ex08: offline: 4.967%(4135)
ext jaccard | tfidf | bought history - 1 times(max==1, ignore) + c->(prob(a)xprob(b)) |

--- ex09: offline: 4.768%(4135)
01 cos | tfidf | bought history - 1 times(max==1, ignore) + c->(prob(a)xprob(b)) |


focus on bought history erase
--- ex10: offline: 
cos | tfidf | bought history top 50 + c->prob(a)xprob(b) |

focus on bought history erase
--- ex11: offline: 
cos | tfidf | bought history top 25 + c->prob(a)xprob(b) |

focus on bought history erase
--- ex12: offline: 
cos | tfidf | bought history top 100 + c->prob(a)xprob(b) |


 ./gao ../data/dim_items.txt ../init/offline_train_item_item.txt ../data/offline_test_items.txt offline_ans.txt

------------------------------------ belows are no use ------------------------



--- ex02: offline: 3.8%(1500)
dot multiply + tfidf + bought history + c ->(max(a), b) b's same list 


--- ex03: offline: 2.xx%
dot multiply + tfidf + c ->(max(a), b) b's same list
