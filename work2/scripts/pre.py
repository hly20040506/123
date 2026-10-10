import json,re,sys,os
W='/home/user/123/work2'
L=json.load(open(f'{W}/list.json'))
gpu_re=re.compile(r'(RTX\s?-?\s?[A-Z]?\d{4}\w*|GTX\s?\d{3,4}\w*|\b[AHLV]\d{2,3}S?\b(?:\s?(?:PCIe|SXM|\d+\s?GB))?|A6000|A5000|A40\b|L40S?|H800|A800|H20\b|H200|B200|GH200|TITAN\s?\w+|Tesla\s?\w+|Quadro\s?\w+|Ascend\s?\w+|TPU\s?v?\d*|Jetson\s?\w+|Apple\s?M\d\w*|MI\d{3}X?|GeForce\s?\w+)')
out=[]
for i,x in enumerate(L,1):
    n=x['pdf'].rsplit('/',1)[1][:-4]
    t=open(f"{W}/txt/{x['id']}_{n}.txt",errors='ignore').read()
    pages=t.split('\f')
    urls=set()
    for m in re.finditer(r'https?://\s?[^\s)\]}>,;]+',t):
        u=re.sub(r'\s','',m.group(0)).rstrip('.')
        urls.add(u)
    gh=sorted(u for u in urls if 'github' in u.lower() or 'huggingface' in u.lower() or 'github.io' in u.lower())
    gpus=[]
    for pi,p in enumerate(pages,1):
        flat=re.sub(r'\s+',' ',p)
        for m in gpu_re.finditer(flat):
            s=max(0,m.start()-90); gpus.append(f"p{pi}: ...{flat[s:m.end()+60]}...")
    code=[]
    for pi,p in enumerate(pages,1):
        flat=re.sub(r'\s+',' ',p)
        for m in re.finditer(r'(code|Code|project page|Project page|will be (made )?(publicly )?(available|released)|open-?source)',flat):
            s=max(0,m.start()-80); code.append(f"p{pi}: ...{flat[s:m.end()+80]}...")
    out.append(dict(idx=i,id=x['id'],pdfnum=n,title=x['title'],dir=x['dir'],npages=len(pages)-1,urls=sorted(urls),gh=gh,gpus=gpus[:12],code=code[:8]))
json.dump(out,open(f'{W}/pre.json','w'),ensure_ascii=False,indent=1)
print(sum(1 for o in out if o['gh']), 'with github/hf links;', sum(1 for o in out if o['gpus']),'with gpu mentions')
