# Created by: Yen-Ming Lee <leeym@FreeBSD.org>
# $FreeBSD$

PORTNAME=	repeater
PORTVERSION=	0.14
CATEGORIES=	net
MASTER_SITES=	http://koti.mbnet.fi/jtko/uvncrepeater/
DISTNAME=	repeater${PORTVERSION:C/\.//}

MAINTAINER=	ports@FreeBSD.org
COMMENT=	UltraVNC repeater

LICENSE=	GPLv2+

BROKEN=		unfetchable
DEPRECATED=	Unfetchable, unmaintained
EXPIRATION_DATE=	2019-10-16

USES=		zip
USE_RC_SUBR=	${PORTNAME}
WRKSRC=		${WRKDIR}/Ver${PORTVERSION:C/\.//}
ALL_TARGET=	${PORTNAME}

REP_USER?=	uvncrep
REP_GROUP?=	uvncrep
USERS=		${REP_USER}
GROUPS=		${REP_GROUP}

SUB_FILES=	pkg-message

PORTDOCS=	changelog.txt readme.txt ultravncrepeaterlinuxport.html

OPTIONS_DEFINE=	DOCS

post-patch:
	@${REINPLACE_CMD} -e 's|g++|${CXX}|' ${WRKSRC}/Makefile

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/${PORTNAME} ${STAGEDIR}${PREFIX}/sbin
	${INSTALL_DATA} ${WRKSRC}/uvncrepeater.ini ${STAGEDIR}${PREFIX}/etc/uvncrepeater.ini.sample

do-install-DOCS-on:
	@${MKDIR} ${STAGEDIR}${DOCSDIR}
	(cd ${WRKSRC} && ${INSTALL_DATA} ${PORTDOCS} ${STAGEDIR}${DOCSDIR})

.include <bsd.port.mk>
