import json,re,sys
W='/home/user/123/work2'
P=json.load(open(f'{W}/pre.json'))
LK=json.load(open(f'{W}/links.json'))
KW=re.compile(r'(pre-?process|normaliz|re-?sampl|downsampl|subsampl|uniformly sampl|randomly sampl|we sample|crop|filter|synthetic|synthesi[sz]ed|pseudo|render(ed)? (from|with|using)|we (select|exclude|remove|discard)|curat|augment|translat(ed|ion) (by|with|using) (GPT|an LLM|LLM)|annotat)',re.I)
for a in sys.argv[1:]:
    o=P[int(a)-1]
    t=open(f"{W}/txt/{o['id']}_{o['pdfnum']}.txt",errors='ignore').read()
    pages=t.split('\f')
    ref=len(pages)
    for pi,p in enumerate(pages):
        if pi>4 and re.search(r'^\s*References\s*$',p,re.M): ref=pi+1;break
    print(f"\n########## #{o['idx']} id={o['id']} pdf={o['pdfnum']} pages={o['npages']} ref={ref}\n{o['title']}\nDIR {o['dir']}")
    print('GH',o['gh']); print('LINKS',[u+'@'+','.join(k+str(p) for k,p in v) for u,v in LK[str(o['idx'])]['links'].items() if any(p<ref for _,p in v)]); print('URLS',[u for u in o['urls'] if not any(k in u for k in ['doi','arxiv','openreview','neurips.cc','semanticscholar','thecvf','acm.org','ieee'])][:12])
    for g in o['gpus'][:6]: print('GPU',g[:260])
    for c in o['code'][:3]: print('CODE',c[:240])
    def comp(s):
        s=re.sub(r'\\@textcolor\s*\{\}\{\w+\}\{\\text\w+\s*\{([^}]*)\}\}',r'\1',s)
        return re.sub(r' {3,}','  ',s)
    n=0
    for pi in range(min(ref,len(pages))):
        flat=re.sub(r'\s+',' ',pages[pi])
        for m in KW.finditer(flat):
            if n>=10: break
            s=max(0,m.start()-160); print(f'KW p{pi+1}: ...{flat[s:m.end()+120]}...'); n+=1
    for pi in range(min(ref,len(pages))):
        lines=[l for l in comp(pages[pi]).split('\n') if l.strip()]
        idxs=[i for i,l in enumerate(lines) if re.match(r'\s*(Table|Tab\.)\s*\d+[.:|]',l)]
        if not idxs: continue
        print(f'\n=== PAGE {pi+1} tables ===')
        shown=set()
        for i in idxs:
            for j in range(max(0,i-16),min(len(lines),i+24)):
                if j not in shown: print(lines[j][:330]); shown.add(j)
            print('  ---')
