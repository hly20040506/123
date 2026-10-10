import json,sys,os
W='/home/user/123/work2'
data=json.loads(sys.stdin.read())
if isinstance(data,dict): data=[data]
for d in data:
    for k in ['idx','dom','proc','gpu','metrics']:
        assert k in d, (d.get('idx'),k)
    assert d['proc'] in ('有','无','未说明')
    json.dump(d,open(f"{W}/results/{d['idx']:03d}.json",'w'),ensure_ascii=False,indent=1)
    print('saved',d['idx'])
print('total',len(os.listdir(f'{W}/results')))
