RM=rm

clean:
	${RM} -rfd part-1-malt-*/malt/build
	${RM} -rfd part-*/cpp/build
	${RM} -rfd part-*/python/__pycache__
	${RM} -rfd part-*/python/.pytest_cache/

clean-corr:
	find ./ -iname "*correction*" -delete
