import tqdm
f = open("онегин", "r")
buff = []
for l in f:
	if len(l) < 60 and not l == "":
		buff.append(l);
f.close()
f = open("онегин_pure", "w")
dictionary = []
for l in tqdm.tqdm(buff):
	if not l in dictionary:
		f.write(l)
		dictionary.append(l)
		
