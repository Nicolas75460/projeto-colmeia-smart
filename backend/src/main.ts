import { NestFactory } from "@nestjs/core";
import { AppModule } from "./app.module";
import { ValidationPipe } from "@nestjs/common";
import { SwaggerModule, DocumentBuilder } from "@nestjs/swagger";

async function bootstrap() {
  const app = await NestFactory.create(AppModule);
  app.useGlobalPipes(new ValidationPipe());
  app.setGlobalPrefix('api');
  const config = new DocumentBuilder()
    .setTitle("API Documentation")
    .setDescription("API description")
    .setVersion("1.0")
    .addBearerAuth(
      {
        type: 'http',
        scheme: 'bearer',
        bearerFormat: 'JWT', // Opcional, apenas informativo
      },
      'bearer' // Nome da chave de segurança
    )
  //   .addSecurity("basic", {
  //     type: "http",
  //     scheme: "basic",
  //   })
  //   .addBasicAuth()
    .build();
  const document = SwaggerModule.createDocument(app, config);
  app.enableCors({
    allowedHeaders: "*",
    origin: "*",
  });
  SwaggerModule.setup("api", app, document);
  await app.listen(3333,'0.0.0.0');
}
bootstrap();
