YEAR=2015
#TARGET_DIR_BASE=$(HOME)/Desktop/MCC-$(YEAR)
TARGET_DIR_BASE=$(HOME)/Desktop/MCC-$(YEAR)
TARGET_WEB_DIR="../../$(YEAR)/web-site/"
TARGET_KIT="$(HOME)/Desktop"
MODELKIT=ModelSubmissionKit
TOOLKIT=ToolSubmissionKit
NUMBER_OF_FORMULA=10

help:
	@echo "################################################################################"
	@echo "# Online help for the MCC Makefile"
	@echo "################################################################################"
	@echo ""
	@echo "List of entries:"
	@echo "   forms          : generates all model forms"
	@echo "   htmltable      : generates the model tables and forms in the web site"
	@echo "   wwebarchive    : generates all model archive (one per model)"
	@echo "   readme         : produced the READ_ME file in the ModelSubmissionKit"
	@echo "   modelkit       : creates the model submission kit"
	@echo "   fixmodels      : fixes models with the Alban's formula utilities"
	@echo "   vmarchive      : produces the model archives for the virtual machine without formulas"
	@echo "   fullvmarchive  : produces the model archives for the virtual machine with formulas included (known only)"
	@echo "   genmccvm       : creates a a default Vm disk image for the MCC "
	@echo "   toolkit        : produces the kit for tool submission "

# Generation des formulaires
forms:
	bash genModelForms.sh "$(TARGET_DIR_BASE)-ModelForms"

# Construction de la table de presentation des modeles sur le site web
htmltable:
	bash model2CSV.sh
	bash csv2htmltable.sh
	make forms
	make webarchives
	mv "$(TARGET_DIR_BASE)-ModelForms"/* $(TARGET_WEB_DIR)/pdf

# Construction des archives PNML (une par modele)
webarchives:
	bash genModelArchive.sh # attention à l'année d'exportation encodée dans le script

# Construction du READ_ME du ModelSubmissionKit
readme:
	pdflatex README.tex
	pdflatex README.tex
	rm -f README.aux README.fls README.out README.synctex.gz README.log README.fdb_latexmk

# Construction du MoelSubmissionKit
modelkit:
	mkdir $(TARGET_KIT)/$(MODELKIT)
	cp background-$(YEAR).jpg $(TARGET_KIT)/$(MODELKIT)
	cp DescModel.tex main.tex $(TARGET_KIT)/$(MODELKIT)
	mkdir $(TARGET_KIT)/$(MODELKIT)/examples
	# --- exemple 1 : HouseConstruction
	mkdir $(TARGET_KIT)/$(MODELKIT)/examples/HouseConstruction
	cp ../HouseConstruction/form/DescModel.tex $(TARGET_KIT)/$(MODELKIT)/examples/HouseConstruction
	cp ../HouseConstruction/form/model-image.pdf $(TARGET_KIT)/$(MODELKIT)/examples/HouseConstruction
	cp -r ../HouseConstruction/PNML $(TARGET_KIT)/$(MODELKIT)/examples/HouseConstruction
	# --- exemple 2 : Vasy2003
	mkdir $(TARGET_KIT)/$(MODELKIT)/examples/Vasy2003
	cp ../Vasy2003/form/DescModel.tex $(TARGET_KIT)/$(MODELKIT)/examples/Vasy2003
	cp -r ../Vasy2003/PNML $(TARGET_KIT)/$(MODELKIT)/examples/Vasy2003
	mkdir $(TARGET_KIT)/$(MODELKIT)/examples/QuasiCertifProtocol
	# --- exemple 3 : QuasiCertifProtocol
	cp ../QuasiCertifProtocol/form/DescModel.tex $(TARGET_KIT)/$(MODELKIT)/examples/QuasiCertifProtocol
	cp ../QuasiCertifProtocol/form/model-image.pdf $(TARGET_KIT)/$(MODELKIT)/examples/QuasiCertifProtocol
	cp -r ../QuasiCertifProtocol/PNML $(TARGET_KIT)/$(MODELKIT)/examples/QuasiCertifProtocol
	# cp READ_ME.txt $(TARGET_KIT)/$(MODELKIT)/
	make readme
	cp README.pdf $(TARGET_KIT)/$(MODELKIT)/
	# suppression des fichiers ._* (ajoutes par OS X) et .svn
	bash cleanKit.sh $(TARGET_KIT)/$(MODELKIT)
	bash -c "cd $(TARGET_KIT) ; tar czf $(MODELKIT).tar.gz $(MODELKIT)"
	cp -f $(TARGET_KIT)/$(MODELKIT).tar.gz $(TARGET_WEB_DIR)/archives
	rm -rf $(TARGET_KIT)/$(MODELKIT)

# Assurer la coherence des identificateurs dans et entre les modeles pour
# produire les formules
fixmodels:
	mcc model:fix /tmp/MCC/BenchKit/INPUTS
	mcc model:check-unfolding /tmp/MCC/BenchKit/INPUTS

# Construire l'archive des modèles pour l'image disque (sans les formules)
vmarchive:
	bash build_vm_archive.sh -r -c -a $(TARGET_DIR_BASE) -known
	bash build_vm_archive.sh -r -c -a $(TARGET_DIR_BASE) -surprise
	scp $(TARGET_DIR_BASE)/MCC-INPUTS.tgz fkordon@pagesperso-systeme.lip6.fr:public_html

# Construire l'archive des modèles pour l'image disque (avec les formules)
fullvmarchive:
	bash build_vm_archive.sh -r $(TARGET_DIR_BASE) -known
	# enlever le -p si la machine n'a pas beaucoup de memoire ou de coeurs
	bash genFormulas.sh -p $(NUMBER_OF_FORMULA)
	bash build_vm_archive.sh -c -a $(TARGET_DIR_BASE) -known
	scp $(TARGET_DIR_BASE)/MCC-INPUTS.tgz fkordon@pagesperso-systeme.lip6.fr:public_html

# Construire l'image disque pour la VM
genmccvm:
	bash -c 'cd ../../$(YEAR)/VM-generation ; bash create_vm_for_mcc.sh'

# Construire le ToolSumissionKit (on suppose l'archive realisee)
toolkit:
	make genmccvm
	mkdir $(TARGET_KIT)/$(TOOLKIT)
	cp $(HOME)/Downloads/mcc$(YEAR).vmdk $(TARGET_KIT)/$(TOOLKIT)
	cp ../../$(YEAR)/VM-generation/mcc/key/mcc2014-key $(TARGET_KIT)/$(TOOLKIT)/bk-private_key
	cp ../../BenchKit2/InSubmissionKit/* $(TARGET_KIT)/$(TOOLKIT)
	bash -c 'cd ../../2014/SubmissionKit/ ; make'
	cp ../../2014/SubmissionKit/MCC2014-SubmissionManual.pdf $(TARGET_KIT)/$(TOOLKIT)/READ_ME.pdf
	bash -c 'cd $(TARGET_KIT) ; export COPYFILE_DISABLE=true ; tar czf $(TOOLKIT).tar.gz $(TOOLKIT)'
	rm -rf $(TARGET_KIT)/$(TOOLKIT)

# finalement pas utilise :-(
atomic-propositions:
	bash build_vm_archive.sh $(TARGET_DIR_BASE) -known
	bash export_cami_from_archive.sh $(TARGET_DIR_BASE)
	bash build_vm_archive.sh $(TARGET_DIR_BASE) -surprise
	bash export_cami_from_archive.sh $(TARGET_DIR_BASE)

