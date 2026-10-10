import json,re,sys
W='/home/user/123/work'
P=json.load(open(f'{W}/pre.json'))
mode=sys.argv[2] if len(sys.argv)>2 else 'tables'
o=P[int(sys.argv[1])-1]
t=open(f"{W}/txt/{o['id']}_{o['pdfnum']}.txt",errors='ignore').read()
pages=t.split('\f')
# find references page
ref=len(pages)
for pi,p in enumerate(pages):
    if pi>4 and re.search(r'^\s*References\s*$',p,re.M): ref=pi+1;break
print(f"#{o['idx']} id={o['id']} pdf={o['pdfnum']} pages={o['npages']} refpage={ref}\n{o['title']}\nDIR {o['dir']}")
print('GH',o['gh']); print('URLS',[u for u in o['urls'] if 'doi.org' not in u and 'arxiv' not in u][:15])
for g in o['gpus'][:8]: print('GPU',g)
for c in o['code'][:5]: print('CODE',c)
def comp(s):
    s=re.sub(r'\\@textcolor\s*\{\}\{\w+\}\{\\text\w+\s*\{([^}]*)\}\}',r'\1',s)
    return re.sub(r' {3,}','  ',s)
if mode=='tables':
    lo=int(sys.argv[3]) if len(sys.argv)>3 else 1
    hi=int(sys.argv[4]) if len(sys.argv)>4 else ref
    for pi in range(lo-1,min(hi,len(pages))):
        p=pages[pi]
        if re.search(r'^\s*Table\s+\d+[.:]',p,re.M) or re.search(r'(Dataset|Implementation|Experimental [Ss]etup|Evaluation [Pp]rotocol|Metrics)',p):
            print(f'\n===== PAGE {pi+1} =====')
            print('\n'.join(l for l in comp(p).split('\n') if l.strip()))
elif mode=='pages':
    for pi in range(int(sys.argv[3])-1,int(sys.argv[4])):
        print(f'\n===== PAGE {pi+1} =====')
        print('\n'.join(l for l in comp(pages[pi]).split('\n') if l.strip()))
elif mode=='grep':
    pat=re.compile(sys.argv[3],re.I)
    for pi,p in enumerate(pages,1):
        flat=re.sub(r'\s+',' ',p)
        for m in pat.finditer(flat):
            s=max(0,m.start()-150); print(f'p{pi}: ...{flat[s:m.end()+150]}...')
